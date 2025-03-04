
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file SensorMag.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#define SENSOR_MAG_ORB_QUEUE_LENGTH 4
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT sensor_mag_s {
#else
struct sensor_mag_s {
#endif // __cplusplus
	uint64_t timestamp;
	uint64_t timestamp_sample;
	uint32_t device_id;
	float x;
	float y;
	float z;
	float temperature;
	uint32_t error_count;

#ifdef __cplusplus
	static constexpr uint8_t ORB_QUEUE_LENGTH = 4;
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using SensorMag = sensor_mag_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(sensor_mag);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const sensor_mag_s& message);
#endif // __cplusplus
