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

#include <lib/perf/perf_counter.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <uORB/PublicationMulti.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/SubscriptionInterval.hpp>
#include <uORB/topics/parameter_update.h>
#include <uORB/topics/sensor_gps.h>
#include <uORB/topics/vehicle_global_position.h>
#include <uORB/topics/vehicle_local_position.h>

using namespace time_literals;

class SensorGpsSim : public ModuleBase<SensorGpsSim>, public ModuleParams, public px4::ScheduledWorkItem {
public:
    SensorGpsSim();
    ~SensorGpsSim() override;

    /** @see ModuleBase */
    static int task_spawn(int argc, char *argv[]);

    /** @see ModuleBase */
    static int custom_command(int argc, char *argv[]);

    /** @see ModuleBase */
    static int print_usage(const char *reason = nullptr);

    bool init();

private:
    void Run() override;

    // generate white Gaussian noise sample with std=1
    static float generate_wgn();

    // generate white Gaussian noise sample as a 3D vector with specified std
    matrix::Vector3f noiseGauss3f(float stdx, float stdy, float stdz) {
        return matrix::Vector3f(generate_wgn() * stdx, generate_wgn() * stdy, generate_wgn() * stdz);
    }

    uORB::SubscriptionInterval _parameter_update_sub{ORB_ID(parameter_update), 1_s};
    uORB::Subscription         _vehicle_global_position_sub{ORB_ID(vehicle_global_position_groundtruth)};
    uORB::Subscription         _vehicle_local_position_sub{ORB_ID(vehicle_local_position_groundtruth)};

    uORB::PublicationMulti<sensor_gps_s> _sensor_gps_pub{ORB_ID(sensor_gps)};

    perf_counter_t _loop_perf{perf_alloc(PC_ELAPSED, MODULE_NAME ": cycle")};

    DEFINE_PARAMETERS(
        (ParamInt<px4::params::SIM_GPS_USED>)_sim_gps_used)
};