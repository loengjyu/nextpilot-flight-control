
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file DifferentialPressure.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT differential_pressure_s {
#else
struct differential_pressure_s {
#endif // __cplusplus
	uint64_t timestamp;
	uint64_t timestamp_sample;
	uint32_t device_id;
	float differential_pressure_pa;
	float temperature;
	uint32_t error_count;

#ifdef __cplusplus
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using DifferentialPressure = differential_pressure_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(differential_pressure);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const differential_pressure_s& message);
#endif // __cplusplus