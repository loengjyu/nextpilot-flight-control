
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file SensorsStatusImu.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT sensors_status_imu_s {
#else
struct sensors_status_imu_s {
#endif // __cplusplus
	uint64_t timestamp;
	uint32_t accel_device_id_primary;
	uint32_t accel_device_ids[4];
	float accel_inconsistency_m_s_s[4];
	uint32_t gyro_device_id_primary;
	uint32_t gyro_device_ids[4];
	float gyro_inconsistency_rad_s[4];
	bool accel_healthy[4];
	uint8_t accel_priority[4];
	bool gyro_healthy[4];
	uint8_t gyro_priority[4];

#ifdef __cplusplus
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using SensorsStatusImu = sensors_status_imu_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(sensors_status_imu);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const sensors_status_imu_s& message);
#endif // __cplusplus