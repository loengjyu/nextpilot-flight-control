
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file YawEstimatorStatus.msg */


#pragma once


#include <uORB.h>


#ifndef __cplusplus

#endif


#ifdef __cplusplus
struct __EXPORT yaw_estimator_status_s {
#else
struct yaw_estimator_status_s {
#endif
	uint64_t timestamp;
	uint64_t timestamp_sample;
	float yaw_composite;
	float yaw_variance;
	float yaw[5];
	float innov_vn[5];
	float innov_ve[5];
	float weight[5];
	bool yaw_composite_valid;
	uint8_t _padding0[7]; // required for logger


#ifdef __cplusplus

#endif
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using YawEstimatorStatus = yaw_estimator_status_s;
	} // namespace msg
} // namespace nextpilot
#endif

/* register this as object request broker structure */
ORB_DECLARE(yaw_estimator_status);


#ifdef __cplusplus
void print_message(const orb_metadata *meta, const yaw_estimator_status_s& message);
#endif