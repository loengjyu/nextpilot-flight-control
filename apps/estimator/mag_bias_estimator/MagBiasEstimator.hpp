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

#include <drivers/drv_hrt.h>
#include <lib/field_sensor_bias_estimator/FieldSensorBiasEstimator.hpp>
#include <lib/mathlib/mathlib.h>
#include <lib/perf/perf_counter.h>
#include <lib/sensor_calibration/Magnetometer.hpp>
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionMultiArray.hpp>
#include <uORB/topics/magnetometer_bias_estimate.h>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/sensor_mag.h>
#include <uORB/topics/vehicle_angular_velocity.h>
#include <uORB/topics/vehicle_status.h>

namespace mag_bias_estimator {

class MagBiasEstimator : public ModuleBase<MagBiasEstimator>, public ModuleParams, public px4::ScheduledWorkItem {
public:
    MagBiasEstimator();
    ~MagBiasEstimator() override;

    static int task_spawn(int argc, char *argv[]);

    /** @see ModuleBase */
    static int custom_command(int argc, char *argv[]) {
        return print_usage("unknown command");
    }

    /** @see ModuleBase */
    static int print_usage(const char *reason = nullptr);

    /** @see ModuleBase::print_status() */
    int print_status() override;

    void start();

private:
    void Run() override;
    void publishMagBiasEstimate();

    static constexpr int MAX_SENSOR_COUNT = 4;

    FieldSensorBiasEstimator _bias_estimator[MAX_SENSOR_COUNT];
    hrt_abstime              _timestamp_last_update[MAX_SENSOR_COUNT]{};

    uORB::SubscriptionMultiArray<sensor_mag_s, MAX_SENSOR_COUNT> _sensor_mag_subs{ORB_ID::sensor_mag};
    uORB::Subscription                                           _parameter_update_sub{ORB_ID(parameter_update)};
    uORB::Subscription                                           _vehicle_angular_velocity_sub{ORB_ID(vehicle_angular_velocity)};
    uORB::Subscription                                           _vehicle_status_sub{ORB_ID(vehicle_status)};

    uORB::Publication<magnetometer_bias_estimate_s> _magnetometer_bias_estimate_pub{ORB_ID(magnetometer_bias_estimate)};

    calibration::Magnetometer _calibration[MAX_SENSOR_COUNT];

    hrt_abstime _time_valid[MAX_SENSOR_COUNT]{};

    bool _reset_field_estimator[MAX_SENSOR_COUNT]{};
    bool _valid[MAX_SENSOR_COUNT]{};

    uint8_t _arming_state{0};
    bool    _system_calibrating{false};

    perf_counter_t _cycle_perf{perf_alloc(PC_ELAPSED, MODULE_NAME ": cycle")};

    DEFINE_PARAMETERS(
        (ParamFloat<px4::params::MBE_LEARN_GAIN>)_param_mbe_learn_gain)
};

} // namespace mag_bias_estimator