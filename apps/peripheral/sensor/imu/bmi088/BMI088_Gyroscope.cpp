/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

#include "BMI088_Gyroscope.hpp"

using namespace time_literals;

namespace Bosch::BMI088::Gyroscope {

BMI088_Gyroscope::BMI088_Gyroscope(const I2CSPIDriverConfig &config) :
    BMI088(config),
    _px4_gyro(get_device_id(), config.rotation) {
    if (config.drdy_gpio != 0) {
        _drdy_missed_perf = perf_alloc(PC_COUNT, MODULE_NAME "_gyro: DRDY missed");
    }

    ConfigureSampleRate(_px4_gyro.get_max_rate_hz());
}

BMI088_Gyroscope::~BMI088_Gyroscope() {
    perf_free(_bad_register_perf);
    perf_free(_bad_transfer_perf);
    perf_free(_fifo_empty_perf);
    perf_free(_fifo_overflow_perf);
    perf_free(_fifo_reset_perf);
    perf_free(_drdy_missed_perf);
}

void BMI088_Gyroscope::exit_and_cleanup() {
    DataReadyInterruptDisable();
    I2CSPIDriverBase::exit_and_cleanup();
}

void BMI088_Gyroscope::print_status() {
    I2CSPIDriverBase::print_status();

    LOG_I("FIFO empty interval: %d us (%.1f Hz)", _fifo_empty_interval_us, 1e6 / _fifo_empty_interval_us);

    perf_print_counter(_bad_register_perf);
    perf_print_counter(_bad_transfer_perf);
    perf_print_counter(_fifo_empty_perf);
    perf_print_counter(_fifo_overflow_perf);
    perf_print_counter(_fifo_reset_perf);
    perf_print_counter(_drdy_missed_perf);
}

int BMI088_Gyroscope::probe() {
    const uint8_t chipid = RegisterRead(Register::GYRO_CHIP_ID);

    if (chipid != ID) {
        DEVICE_DEBUG("unexpected GYRO_CHIP_ID 0x%02x", chipid);
        return RT_ERROR;
    }

    return RT_EOK;
}

void BMI088_Gyroscope::RunImpl() {
    const hrt_abstime now = hrt_absolute_time();

    switch (_state) {
    case STATE::RESET:
        // GYRO_SOFTRESET: Writing a value of 0xB6 to this register resets the sensor.
        // Following a delay of 30 ms, all configuration settings are overwritten with their reset value.
        RegisterWrite(Register::GYRO_SOFTRESET, 0xB6);
        DataReadyInterruptDisable();
        _reset_timestamp = now;
        _failure_count   = 0;
        _state           = STATE::WAIT_FOR_RESET;
        ScheduleDelayed(30_ms);
        break;

    case STATE::WAIT_FOR_RESET:
        if ((RegisterRead(Register::GYRO_CHIP_ID) == ID)) {
            // if reset succeeded then configure
            _state = STATE::CONFIGURE;
            ScheduleDelayed(10_ms);

        } else {
            // RESET not complete
            if (hrt_elapsed_time(&_reset_timestamp) > 1000_ms) {
                LOG_D("Reset failed, retrying");
                _state = STATE::RESET;
                ScheduleDelayed(100_ms);

            } else {
                LOG_D("Reset not complete, check again in 10 ms");
                ScheduleDelayed(10_ms);
            }
        }

        break;

    case STATE::CONFIGURE:
        if (Configure()) {
            // if configure succeeded then reset the FIFO
            _state = STATE::FIFO_RESET;
            ScheduleDelayed(10_ms);

        } else {
            // CONFIGURE not complete
            if (hrt_elapsed_time(&_reset_timestamp) > 1000_ms) {
                LOG_D("Configure failed, resetting");
                _state = STATE::RESET;

            } else {
                LOG_D("Configure failed, retrying");
            }

            ScheduleDelayed(100_ms);
        }

        break;

    case STATE::FIFO_RESET:
        // if configure succeeded then start reading from FIFO
        _state = STATE::FIFO_READ;

        FIFOReset();

        if (DataReadyInterruptConfigure()) {
            _data_ready_interrupt_enabled = true;

            // backup schedule as a watchdog timeout
            ScheduleDelayed(100_ms);

        } else {
            _data_ready_interrupt_enabled = false;
            ScheduleOnInterval(_fifo_empty_interval_us, _fifo_empty_interval_us);
        }

        break;

    case STATE::FIFO_READ: {
        hrt_abstime timestamp_sample = 0;

        if (_data_ready_interrupt_enabled) {
            // scheduled from interrupt if _drdy_timestamp_sample was set as expected
            const hrt_abstime drdy_timestamp_sample = _drdy_timestamp_sample.fetch_and(0);

            if ((now - drdy_timestamp_sample) < _fifo_empty_interval_us) {
                timestamp_sample = drdy_timestamp_sample;

            } else {
                perf_count(_drdy_missed_perf);
            }

            // push backup schedule back
            ScheduleDelayed(_fifo_empty_interval_us * 2);
        }

        // always check current FIFO status/count
        bool          success     = false;
        const uint8_t FIFO_STATUS = RegisterRead(Register::FIFO_STATUS);

        if (FIFO_STATUS & FIFO_STATUS_BIT::Fifo_overrun) {
            FIFOReset();
            perf_count(_fifo_overflow_perf);

        } else {
            const uint8_t fifo_frame_counter = FIFO_STATUS & FIFO_STATUS_BIT::Fifo_frame_counter;

            if (fifo_frame_counter > FIFO_MAX_SAMPLES) {
                // not technically an overflow, but more samples than we expected or can publish
                FIFOReset();
                perf_count(_fifo_overflow_perf);

            } else if (fifo_frame_counter == 0) {
                perf_count(_fifo_empty_perf);

            } else if (fifo_frame_counter >= 1) {
                uint8_t samples = fifo_frame_counter;

                // tolerate minor jitter, leave sample to next iteration if behind by only 1
                if (samples == _fifo_samples + 1) {
                    // sample timestamp set from data ready already corresponds to _fifo_samples
                    if (timestamp_sample == 0) {
                        timestamp_sample = now - static_cast<int>(FIFO_SAMPLE_DT);
                    }

                    samples--;
                }

                if (FIFORead((timestamp_sample == 0) ? now : timestamp_sample, samples)) {
                    success = true;

                    if (_failure_count > 0) {
                        _failure_count--;
                    }
                }
            }
        }

        if (!success) {
            _failure_count++;

            // full reset if things are failing consistently
            if (_failure_count > 10) {
                Reset();
                return;
            }
        }

        if (!success || hrt_elapsed_time(&_last_config_check_timestamp) > 100_ms) {
            // check configuration registers periodically or immediately following any failure
            if (RegisterCheck(_register_cfg[_checked_register])) {
                _last_config_check_timestamp = now;
                _checked_register            = (_checked_register + 1) % size_register_cfg;

            } else {
                // register check failed, force reset
                perf_count(_bad_register_perf);
                Reset();
            }
        }
    }

    break;
    }
}

void BMI088_Gyroscope::ConfigureGyro() {
    const uint8_t GYRO_RANGE = RegisterRead(Register::GYRO_RANGE) & (Bit3 | Bit2 | Bit1 | Bit0);

    switch (GYRO_RANGE) {
    case gyro_range_2000_dps:
        _px4_gyro.set_scale(math::radians(1.f / 16.384f));
        _px4_gyro.set_range(math::radians(2000.f));
        break;

    case gyro_range_1000_dps:
        _px4_gyro.set_scale(math::radians(1.f / 32.768f));
        _px4_gyro.set_range(math::radians(1000.f));
        break;

    case gyro_range_500_dps:
        _px4_gyro.set_scale(math::radians(1.f / 65.536f));
        _px4_gyro.set_range(math::radians(500.f));
        break;

    case gyro_range_250_dps:
        _px4_gyro.set_scale(math::radians(1.f / 131.072f));
        _px4_gyro.set_range(math::radians(250.f));
        break;

    case gyro_range_125_dps:
        _px4_gyro.set_scale(math::radians(1.f / 262.144f));
        _px4_gyro.set_range(math::radians(125.f));
        break;
    }
}

void BMI088_Gyroscope::ConfigureSampleRate(int sample_rate) {
    // round down to nearest FIFO sample dt * SAMPLES_PER_TRANSFER
    const float min_interval = FIFO_SAMPLE_DT;
    _fifo_empty_interval_us  = math::max(roundf((1e6f / (float)sample_rate) / min_interval) * min_interval, min_interval);

    _fifo_samples = math::min((float)_fifo_empty_interval_us / (1e6f / RATE), (float)FIFO_MAX_SAMPLES);

    // recompute FIFO empty interval (us) with actual sample limit
    _fifo_empty_interval_us = _fifo_samples * (1e6f / RATE);

    ConfigureFIFOWatermark(_fifo_samples);
}

void BMI088_Gyroscope::ConfigureFIFOWatermark(uint8_t samples) {
    // FIFO watermark threshold
    for (auto &r : _register_cfg) {
        if (r.reg == Register::FIFO_CONFIG_0) {
            r.set_bits   = samples;
            r.clear_bits = ~r.set_bits;
        }
    }
}

bool BMI088_Gyroscope::Configure() {
    // first set and clear all configured register bits
    for (const auto &reg_cfg : _register_cfg) {
        RegisterSetAndClearBits(reg_cfg.reg, reg_cfg.set_bits, reg_cfg.clear_bits);
    }

    // now check that all are configured
    bool success = true;

    for (const auto &reg_cfg : _register_cfg) {
        if (!RegisterCheck(reg_cfg)) {
            success = false;
        }
    }

    ConfigureGyro();

    return success;
}

int BMI088_Gyroscope::DataReadyInterruptCallback(int irq, void *context, void *arg) {
    static_cast<BMI088_Gyroscope *>(arg)->DataReady();
    return 0;
}

void BMI088_Gyroscope::DataReady() {
    _drdy_timestamp_sample.store(hrt_absolute_time());
    ScheduleNow();
}

bool BMI088_Gyroscope::DataReadyInterruptConfigure() {
    if (_drdy_gpio == 0) {
        return false;
    }

    // Setup data ready on falling edge
    return px4_arch_gpiosetevent(_drdy_gpio, false, true, true, &DataReadyInterruptCallback, this) == 0;
}

bool BMI088_Gyroscope::DataReadyInterruptDisable() {
    if (_drdy_gpio == 0) {
        return false;
    }

    return px4_arch_gpiosetevent(_drdy_gpio, false, false, false, nullptr, nullptr) == 0;
}

bool BMI088_Gyroscope::RegisterCheck(const register_config_t &reg_cfg) {
    bool success = true;

    const uint8_t reg_value = RegisterRead(reg_cfg.reg);

    if (reg_cfg.set_bits && ((reg_value & reg_cfg.set_bits) != reg_cfg.set_bits)) {
        LOG_D("0x%02hhX: 0x%02hhX (0x%02hhX not set)", (uint8_t)reg_cfg.reg, reg_value, reg_cfg.set_bits);
        success = false;
    }

    if (reg_cfg.clear_bits && ((reg_value & reg_cfg.clear_bits) != 0)) {
        LOG_D("0x%02hhX: 0x%02hhX (0x%02hhX not cleared)", (uint8_t)reg_cfg.reg, reg_value, reg_cfg.clear_bits);
        success = false;
    }

    return success;
}

uint8_t BMI088_Gyroscope::RegisterRead(Register reg) {
    uint8_t cmd[2]{};
    cmd[0] = static_cast<uint8_t>(reg) | DIR_READ;
    transfer(cmd, cmd, sizeof(cmd));
    return cmd[1];
}

void BMI088_Gyroscope::RegisterWrite(Register reg, uint8_t value) {
    uint8_t cmd[2]{(uint8_t)reg, value};
    transfer(cmd, cmd, sizeof(cmd));
}

void BMI088_Gyroscope::RegisterSetAndClearBits(Register reg, uint8_t setbits, uint8_t clearbits) {
    const uint8_t orig_val = RegisterRead(reg);

    uint8_t val = (orig_val & ~clearbits) | setbits;

    if (orig_val != val) {
        RegisterWrite(reg, val);
    }
}

bool BMI088_Gyroscope::FIFORead(const hrt_abstime &timestamp_sample, uint8_t samples) {
    FIFOTransferBuffer buffer{};
    const size_t       transfer_size = math::min(samples * sizeof(FIFO::DATA) + 1, FIFO::SIZE);

    if (transfer((uint8_t *)&buffer, (uint8_t *)&buffer, transfer_size) != RT_EOK) {
        perf_count(_bad_transfer_perf);
        return false;
    }

    sensor_gyro_fifo_s gyro{};
    gyro.timestamp_sample = timestamp_sample;
    gyro.samples          = samples;
    gyro.dt               = FIFO_SAMPLE_DT;

    int index = 0;

    for (int i = 0; i < samples; i++) {
        const FIFO::DATA &fifo_sample = buffer.f[i];

        const int16_t gyro_x = combine(fifo_sample.RATE_X_MSB, fifo_sample.RATE_X_LSB);
        const int16_t gyro_y = combine(fifo_sample.RATE_Y_MSB, fifo_sample.RATE_Y_LSB);
        const int16_t gyro_z = combine(fifo_sample.RATE_Z_MSB, fifo_sample.RATE_Z_LSB);

        // sensor's frame is +x forward, +y left, +z up
        //  flip y & z to publish right handed with z down (x forward, y right, z down)
        if (!(gyro_x == INT16_MIN && gyro_y == INT16_MIN && gyro_z == INT16_MIN)) {
            gyro.x[index] = gyro_x;
            gyro.y[index] = (gyro_y == INT16_MIN) ? INT16_MAX : -gyro_y;
            gyro.z[index] = (gyro_z == INT16_MIN) ? INT16_MAX : -gyro_z;
            ++index;
        }
    }

    _px4_gyro.set_error_count(perf_event_count(_bad_register_perf) + perf_event_count(_bad_transfer_perf) + perf_event_count(_fifo_empty_perf) + perf_event_count(_fifo_overflow_perf));

    if (index > 0) {
        _px4_gyro.updateFIFO(gyro);
    }

    return true;
}

void BMI088_Gyroscope::FIFOReset() {
    perf_count(_fifo_reset_perf);

    // FIFO_CONFIG_0: Writing to water mark level trigger in register 0x3D (FIFO_CONFIG_0) clears the FIFO buffer.
    RegisterWrite(Register::FIFO_CONFIG_0, 0);

    // FIFO_CONFIG_1: FIFO overrun condition can only be cleared by writing to the FIFO configuration register FIFO_CONFIG_1
    RegisterWrite(Register::FIFO_CONFIG_1, 0);

    // reset while FIFO is disabled
    _drdy_timestamp_sample.store(0);

    // FIFO_CONFIG_0: restore FIFO watermark
    // FIFO_CONFIG_1: re-enable FIFO
    for (const auto &r : _register_cfg) {
        if ((r.reg == Register::FIFO_CONFIG_0) || (r.reg == Register::FIFO_CONFIG_1)) {
            RegisterSetAndClearBits(r.reg, r.set_bits, r.clear_bits);
        }
    }
}

} // namespace Bosch::BMI088::Gyroscope
