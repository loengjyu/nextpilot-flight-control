
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file NormalizedUnsignedSetpoint.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT normalized_unsigned_setpoint_s {
#else
struct normalized_unsigned_setpoint_s {
#endif // __cplusplus
	uint64_t timestamp;
	float normalized_setpoint;
	uint8_t _padding0[4]; // required for logger

#ifdef __cplusplus
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using NormalizedUnsignedSetpoint = normalized_unsigned_setpoint_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(flaps_setpoint);
ORB_DECLARE(spoilers_setpoint);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const normalized_unsigned_setpoint_s& message);
#endif // __cplusplus