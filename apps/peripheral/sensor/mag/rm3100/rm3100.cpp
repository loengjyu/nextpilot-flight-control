/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/**
 * @file rm3100.cpp
 *
 * Driver for the RM3100 magnetometer connected via I2C or SPI.
 *
 * Based on the lis3mdl driver.
 */

#include "rm3100.h"

RM3100::RM3100(device::Device *interface, const I2CSPIDriverConfig &config) :
    I2CSPIDriver(config),
    _px4_mag(interface->get_device_id(), config.rotation),
    _interface(interface) {
    _px4_mag.set_scale(1.f / (RM3100_SENSITIVITY * UTESLA_TO_GAUSS));
}

RM3100::~RM3100() {
    // free perf counters
    perf_free(_reset_perf);
    perf_free(_range_error_perf);
    perf_free(_bad_transfer_perf);

    delete _interface;
}

int RM3100::self_test() {
    bool complete = false;

    // Set the default command mode and enable polling (not continuous mode)
    uint8_t cmd = (CMM_DEFAULT & ~CONTINUOUS_MODE);
    int     ret = _interface->write(ADDR_CMM, &cmd, 1);

    if (ret != RT_EOK) {
        return ret;
    }

    cmd = HSHAKE_NO_DRDY_CLEAR;
    ret = _interface->write(ADDR_HSHAKE, &cmd, 1);

    if (ret != RT_EOK) {
        return ret;
    }

    // Configure sensor to execute BIST upon receipt of a POLL command
    cmd = BIST_SELFTEST;
    ret = _interface->write(ADDR_BIST, &cmd, 1);

    if (ret != RT_EOK) {
        return ret;
    }

    // Poll to start the self test
    cmd = POLL_XYZ;
    ret = _interface->write(ADDR_POLL, &cmd, 1);

    if (ret != RT_EOK) {
        return ret;
    }

    // Perform test procedure until a valid result is obtained or test times out

    const hrt_abstime t_start = hrt_absolute_time();

    while ((hrt_absolute_time() - t_start) < BIST_DUR_USEC) {
        uint8_t status = 0;
        ret            = _interface->read(ADDR_STATUS, &status, 1);

        if (ret != RT_EOK) {
            return ret;
        }

        // If the DRDY bit in the status register is set, BIST should be complete
        if (status & STATUS_DRDY) {
            // Check BIST register to evaluate the test result
            ret = _interface->read(ADDR_BIST, &cmd, 1);

            if (ret != RT_EOK) {
                return ret;
            }

            // The test results are not valid if STE is not set. In this case, we try again
            if (cmd & BIST_STE) {
                complete = true;

                // If the x, y, or z LR oscillators malfunctioned then the self test failed.
                if ((cmd & BIST_XYZ_OK) ^ BIST_XYZ_OK) {
                    LOG_E("built-in self test failed: 0x%2X x:%s y:%s z:%s", cmd,
                          cmd & 0x10 ? "Pass" : "Fail",
                          cmd & 0x20 ? "Pass" : "Fail",
                          cmd & 0x40 ? "Pass" : "Fail");
                    return RT_ERROR;

                } else {
                    // The test passed, disable self-test mode by clearing the STE bit
                    cmd = 0;
                    ret = _interface->write(ADDR_BIST, &cmd, 1);

                    if (ret != RT_EOK) {
                        LOG_E("Failed to disable built-in self test");
                    }

                    return RT_EOK;
                }
            }
        }
    }

    if (!complete) {
        LOG_E("built-in self test incomplete");
    }

    return RT_ERROR;
}

void RM3100::RunImpl() {
    // full reset if things are failing consistently
    if (_failure_count > 10) {
        _failure_count = 0;
        set_default_register_values();
        ScheduleOnInterval(RM3100_INTERVAL);
        return;
    }

    struct {
        uint8_t x[3];
        uint8_t y[3];
        uint8_t z[3];
    } rm_report{};

    const hrt_abstime timestamp_sample = hrt_absolute_time();
    int               ret              = _interface->read(ADDR_MX, (uint8_t *)&rm_report, sizeof(rm_report));

    if (ret != OK) {
        perf_count(_bad_transfer_perf);
        _failure_count++;
        return;
    }

    /* Rearrange mag data */
    int32_t xraw = ((rm_report.x[0] << 16) | (rm_report.x[1] << 8) | rm_report.x[2]);
    int32_t yraw = ((rm_report.y[0] << 16) | (rm_report.y[1] << 8) | rm_report.y[2]);
    int32_t zraw = ((rm_report.z[0] << 16) | (rm_report.z[1] << 8) | rm_report.z[2]);

    /* Convert 24 bit signed values to 32 bit signed values */
    convert_signed(&xraw);
    convert_signed(&yraw);
    convert_signed(&zraw);

    // valid range: -8388608 to 8388607
    if (xraw < -8388608 || xraw > 8388607 || yraw < -8388608 || yraw > 8388607 || zraw < -8388608 || zraw > 8388607) {
        _failure_count++;

        perf_count(_range_error_perf);
        return;
    }

    // only publish changes
    if (_raw_data_prev[0] != xraw || _raw_data_prev[1] != yraw || _raw_data_prev[2] != zraw) {
        _px4_mag.set_error_count(perf_event_count(_bad_transfer_perf)
                                 + perf_event_count(_range_error_perf));

        _px4_mag.update(timestamp_sample, xraw, yraw, zraw);

        _raw_data_prev[0] = xraw;
        _raw_data_prev[1] = yraw;
        _raw_data_prev[2] = zraw;

        if (_failure_count > 0) {
            _failure_count--;
        }

    } else {
        _failure_count++;
    }
}

void RM3100::convert_signed(int32_t *n) {
    /* Sensor returns values as 24 bit signed values, so we need to manually convert to 32 bit signed values */
    if ((*n & (1 << 23)) == (1 << 23)) {
        *n |= 0xFF000000;
    }
}

int RM3100::init() {
    int ret = self_test();

    if (ret != RT_EOK) {
        LOG_E("self test failed");
    }

    if (set_default_register_values() == RT_EOK) {
        ScheduleOnInterval(RM3100_INTERVAL);
        return RT_EOK;
    }

    return RT_ERROR;
}

void RM3100::print_status() {
    I2CSPIDriverBase::print_status();
    perf_print_counter(_reset_perf);
    perf_print_counter(_range_error_perf);
    perf_print_counter(_bad_transfer_perf);
}

int RM3100::set_default_register_values() {
    perf_count(_reset_perf);

    uint8_t cmd[2] = {0, 0};

    cmd[0] = CCX_DEFAULT_MSB;
    cmd[1] = CCX_DEFAULT_LSB;
    _interface->write(ADDR_CCX, cmd, 2);

    cmd[0] = CCY_DEFAULT_MSB;
    cmd[1] = CCY_DEFAULT_LSB;
    _interface->write(ADDR_CCY, cmd, 2);

    cmd[0] = CCZ_DEFAULT_MSB;
    cmd[1] = CCZ_DEFAULT_LSB;
    _interface->write(ADDR_CCZ, cmd, 2);

    cmd[0] = HSHAKE_DEFAULT;
    _interface->write(ADDR_HSHAKE, cmd, 1);

    cmd[0] = CMM_DEFAULT;
    _interface->write(ADDR_CMM, cmd, 1);

    cmd[0] = TMRC_DEFAULT;
    _interface->write(ADDR_TMRC, cmd, 1);

    cmd[0] = BIST_DEFAULT;
    _interface->write(ADDR_BIST, cmd, 1);

    return RT_EOK;
}
