
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file Rpm.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT rpm_s {
#else
struct rpm_s {
#endif // __cplusplus
	uint64_t timestamp;
	float indicated_frequency_rpm;
	float estimated_accurancy_rpm;

#ifdef __cplusplus
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using Rpm = rpm_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(rpm);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const rpm_s& message);
#endif // __cplusplus
