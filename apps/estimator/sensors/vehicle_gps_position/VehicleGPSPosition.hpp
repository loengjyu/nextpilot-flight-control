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

#include <lib/mathlib/math/Limits.hpp>
#include <lib/matrix/matrix/math.hpp>
#include <lib/perf/perf_counter.h>
#include <px4_platform_common/log.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionCallback.hpp>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/sensor_gps.h>

#include "gps_blending.hpp"

using namespace time_literals;

namespace sensors {
class VehicleGPSPosition : public ModuleParams, public px4::ScheduledWorkItem {
public:
    VehicleGPSPosition();
    ~VehicleGPSPosition() override;

    bool Start();
    void Stop();

    void PrintStatus();

private:
    void Run() override;

    void ParametersUpdate(bool force = false);

    // defines used to specify the mask position for use of different accuracy metrics in the GPS blending algorithm
    static constexpr uint8_t BLEND_MASK_USE_SPD_ACC  = 1;
    static constexpr uint8_t BLEND_MASK_USE_HPOS_ACC = 2;
    static constexpr uint8_t BLEND_MASK_USE_VPOS_ACC = 4;

    // define max number of GPS receivers supported
    static constexpr int GPS_MAX_RECEIVERS = 2;
    static_assert(GPS_MAX_RECEIVERS == GpsBlending::GPS_MAX_RECEIVERS_BLEND,
                  "GPS_MAX_RECEIVERS must match to GPS_MAX_RECEIVERS_BLEND");

    uORB::Publication<sensor_gps_s> _vehicle_gps_position_pub{ORB_ID(vehicle_gps_position)};

    uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};

    uORB::SubscriptionCallbackWorkItem _sensor_gps_sub[GPS_MAX_RECEIVERS]{
        /**< sensor data subscription */
        {this, ORB_ID(sensor_gps), 0},
        {this, ORB_ID(sensor_gps), 1},
    };

    perf_counter_t _cycle_perf{perf_alloc(PC_ELAPSED, MODULE_NAME ": cycle")};

    GpsBlending _gps_blending;

    DEFINE_PARAMETERS(
        (ParamInt<px4::params::SENS_GPS_MASK>)_param_sens_gps_mask,
        (ParamFloat<px4::params::SENS_GPS_TAU>)_param_sens_gps_tau,
        (ParamInt<px4::params::SENS_GPS_PRIME>)_param_sens_gps_prime)
};
}; // namespace sensors