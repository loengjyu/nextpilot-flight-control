/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

#pragma once

#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/posix.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <drivers/drv_hrt.h>
#include <lib/mathlib/math/WelfordMeanVector.hpp>
#include <lib/perf/perf_counter.h>
#include <lib/sensor_calibration/Gyroscope.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionInterval.hpp>
#include <uORB/SubscriptionMultiArray.hpp>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/sensor_accel.h>
#include <uORB/topics/sensor_gyro.h>
#include <uORB/topics/vehicle_status.h>

using namespace time_literals;

class GyroCalibration : public ModuleBase<GyroCalibration>, public ModuleParams, public px4::ScheduledWorkItem {
public:
    GyroCalibration();
    ~GyroCalibration() override;

    /** @see ModuleBase */
    static int task_spawn(int argc, char *argv[]);

    /** @see ModuleBase */
    static int custom_command(int argc, char *argv[]);

    /** @see ModuleBase */
    static int print_usage(const char *reason = nullptr);

    bool init();

    int print_status() override;

private:
    static constexpr hrt_abstime INTERVAL_US = 20000_us;
    static constexpr int         MAX_SENSORS = 4;

    void Run() override;

    void Reset() {
        for (int gyro = 0; gyro < MAX_SENSORS; gyro++) {
            _gyro_mean[gyro].reset();
            _gyro_last_update[gyro] = 0;
        }

        _last_calibration_update = hrt_absolute_time();
    }

    // return the square of two floating point numbers
    static constexpr float sq(float var) {
        return var * var;
    }

    uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};
    uORB::Subscription         _vehicle_status_sub{ORB_ID::vehicle_status};

    uORB::SubscriptionMultiArray<sensor_accel_s, MAX_SENSORS> _sensor_accel_subs{ORB_ID::sensor_accel};
    uORB::SubscriptionMultiArray<sensor_gyro_s, MAX_SENSORS>  _sensor_gyro_subs{ORB_ID::sensor_gyro};

    calibration::Gyroscope            _gyro_calibration[MAX_SENSORS]{};
    math::WelfordMeanVector<float, 3> _gyro_mean[MAX_SENSORS]{};
    matrix::Vector3f                  _gyro_cal_variance[MAX_SENSORS]{};
    float                             _temperature[MAX_SENSORS]{};
    hrt_abstime                       _gyro_last_update[MAX_SENSORS]{};

    matrix::Vector3f _acceleration[MAX_SENSORS]{};

    hrt_abstime _last_calibration_update{0};

    bool _armed{false};
    bool _system_calibrating{false};

    perf_counter_t _loop_interval_perf{perf_alloc(PC_INTERVAL, MODULE_NAME ": interval")};
    perf_counter_t _calibration_updated_perf{perf_alloc(PC_COUNT, MODULE_NAME ": calibration updated")};
};