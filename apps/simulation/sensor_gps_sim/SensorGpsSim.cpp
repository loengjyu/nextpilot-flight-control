/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

#define LOG_TAG "sim_gps"

#include "SensorGpsSim.hpp"
#include <device/device_id.h>
#include <geo/geo.h>

using namespace matrix;

SensorGpsSim::SensorGpsSim() :
    ModuleParams(nullptr),
    WorkItemScheduled(MODULE_NAME, wq_configurations::hp_default) {
}

SensorGpsSim::~SensorGpsSim() {
    perf_free(_loop_perf);
}

int SensorGpsSim::init() {
    ScheduleOnInterval(125_ms); // 8 Hz
    return 0;
}

float SensorGpsSim::generate_wgn() {
    // generate white Gaussian noise sample with std=1

    // algorithm 1:
    // float temp=((float)(rand()+1))/(((float)RAND_MAX+1.0f));
    // return sqrtf(-2.0f*logf(temp))*cosf(2.0f*M_PI_F*rand()/RAND_MAX);
    // algorithm 2: from BlockRandGauss.hpp
    static float V1, V2, S;
    static bool  phase = true;
    float        X;

    if (phase) {
        do {
            float U1 = (float)rand() / (float)RAND_MAX;
            float U2 = (float)rand() / (float)RAND_MAX;
            V1       = 2.0f * U1 - 1.0f;
            V2       = 2.0f * U2 - 1.0f;
            S        = V1 * V1 + V2 * V2;
        } while (S >= 1.0f || fabsf(S) < 1e-8f);

        X = V1 * float(sqrtf(-2.0f * float(logf(S)) / S));

    } else {
        X = V2 * float(sqrtf(-2.0f * float(logf(S)) / S));
    }

    phase = !phase;
    return X;
}

void SensorGpsSim::Run() {
    if (should_exit()) {
        ScheduleClear();
        exit_and_cleanup();
        return;
    }

    perf_begin(_loop_perf);

    // Check if parameters have changed
    if (_parameter_update_sub.updated()) {
        // clear update
        parameter_update_s param_update;
        _parameter_update_sub.copy(&param_update);

        updateParams();
    }

    if (_vehicle_local_position_sub.updated() && _vehicle_global_position_sub.updated()) {
        vehicle_local_position_s lpos{};
        _vehicle_local_position_sub.copy(&lpos);

        vehicle_global_position_s gpos{};
        _vehicle_global_position_sub.copy(&gpos);

        double latitude  = gpos.lat + math::degrees((double)generate_wgn() * 0.2 / CONSTANTS_RADIUS_OF_EARTH);
        double longitude = gpos.lon + math::degrees((double)generate_wgn() * 0.2 / CONSTANTS_RADIUS_OF_EARTH);
        float  altitude  = gpos.alt + (generate_wgn() * 0.5f);

        Vector3f gps_vel = Vector3f{lpos.vx, lpos.vy, lpos.vz} + noiseGauss3f(0.06f, 0.077f, 0.158f);

        // device id
        device::DeviceId device_id;
        device_id.devid_s.bus_type  = device::DeviceBusType::DeviceBusType_SIMULATION;
        device_id.devid_s.bus_index = 0;
        device_id.devid_s.address   = 0;
        device_id.devid_s.devtype   = DRV_GPS_DEVTYPE_SIM;

        sensor_gps_s sensor_gps{};

        if (_sim_gps_used.get() >= 4) {
            // fix
            sensor_gps.fix_type       = 3; // 3D fix
            sensor_gps.s_variance_m_s = 0.5f;
            sensor_gps.c_variance_rad = 0.1f;
            sensor_gps.eph            = 0.9f;
            sensor_gps.epv            = 1.78f;
            sensor_gps.hdop           = 0.7f;
            sensor_gps.vdop           = 1.1f;

        } else {
            // no fix
            sensor_gps.fix_type       = 0; // No fix
            sensor_gps.s_variance_m_s = 100.f;
            sensor_gps.c_variance_rad = 100.f;
            sensor_gps.eph            = 100.f;
            sensor_gps.epv            = 100.f;
            sensor_gps.hdop           = 100.f;
            sensor_gps.vdop           = 100.f;
        }

        sensor_gps.timestamp_sample        = gpos.timestamp_sample;
        sensor_gps.time_utc_usec           = 0;
        sensor_gps.device_id               = device_id.devid;
        sensor_gps.lat                     = roundf(latitude * 1e7);    // Latitude in 1E-7 degrees
        sensor_gps.lon                     = roundf(longitude * 1e7);   // Longitude in 1E-7 degrees
        sensor_gps.alt                     = roundf(altitude * 1000.f); // Altitude in 1E-3 meters above MSL, (millimetres)
        sensor_gps.alt_ellipsoid           = sensor_gps.alt;
        sensor_gps.noise_per_ms            = 0;
        sensor_gps.jamming_indicator       = 0;
        sensor_gps.vel_m_s                 = sqrtf(gps_vel(0) * gps_vel(0) + gps_vel(1) * gps_vel(1)); // GPS ground speed, (metres/sec)
        sensor_gps.vel_n_m_s               = gps_vel(0);
        sensor_gps.vel_e_m_s               = gps_vel(1);
        sensor_gps.vel_d_m_s               = gps_vel(2);
        sensor_gps.cog_rad                 = atan2(gps_vel(1),
                                                   gps_vel(0)); // Course over ground (NOT heading, but direction of movement), -PI..PI, (radians)
        sensor_gps.timestamp_time_relative = 0;
        sensor_gps.heading                 = NAN;
        sensor_gps.heading_offset          = NAN;
        sensor_gps.heading_accuracy        = 0;
        sensor_gps.automatic_gain_control  = 0;
        sensor_gps.jamming_state           = 0;
        sensor_gps.spoofing_state          = 0;
        sensor_gps.vel_ned_valid           = true;
        sensor_gps.satellites_used         = _sim_gps_used.get();

        sensor_gps.timestamp = hrt_absolute_time();
        _sensor_gps_pub.publish(sensor_gps);
    }

    perf_end(_loop_perf);
}

SensorGpsSim *SensorGpsSim::instantiate(int argc, char *argv[]) {
    SensorGpsSim *instance = new SensorGpsSim();

    // if (instance) {
    //     _object.store(instance);
    //     _task_id = task_id_is_work_queue;

    //     if (instance->init()) {
    //         return PX4_OK;
    //     }

    // } else {
    //     PX4_ERR("alloc failed");
    // }

    // delete instance;
    // _object.store(nullptr);
    // _task_id = -1;

    // return PX4_ERROR;

    return instance;
}

int SensorGpsSim::custom_command(int argc, char *argv[]) {
    return print_usage("unknown command");
}

int SensorGpsSim::print_usage(const char *reason) {
    if (reason) {
        PX4_WARN("%s\n", reason);
    }

    PRINT_MODULE_DESCRIPTION(
        R"DESCR_STR(
### Description


)DESCR_STR");

    PRINT_MODULE_USAGE_NAME("sensor_gps_sim", "system");
    PRINT_MODULE_USAGE_COMMAND("start");
    PRINT_MODULE_USAGE_DEFAULT_COMMANDS();

    return 0;
}

extern "C" __EXPORT int sensor_gps_sim_main(int argc, char *argv[]) {
    return SensorGpsSim::main(argc, argv);
}

MSH_CMD_EXPORT_ALIAS(sensor_gps_sim_main, sim_gps, gps simulator);

int sensor_gps_sim_start() {
    int32_t hitl = param_get_int32((param_t)params_id::SYS_HITL);

    if (hitl == 2) {
        const char *argv[] = {"sim_gps", "start"};
        int         argc   = sizeof(argv) / sizeof(argv[0]);
        return SensorGpsSim::main(argc, (char **)argv);
    }

    return 0;
}

INIT_APP_EXPORT(sensor_gps_sim_start);
