
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file EstimatorGpsStatus.msg */


#pragma once


#include <uORB.h>


#ifndef __cplusplus

#endif


#ifdef __cplusplus
struct __EXPORT estimator_gps_status_s {
#else
struct estimator_gps_status_s {
#endif
	uint64_t timestamp;
	uint64_t timestamp_sample;
	float position_drift_rate_horizontal_m_s;
	float position_drift_rate_vertical_m_s;
	float filtered_horizontal_speed_m_s;
	bool checks_passed;
	bool check_fail_gps_fix;
	bool check_fail_min_sat_count;
	bool check_fail_max_pdop;
	bool check_fail_max_horz_err;
	bool check_fail_max_vert_err;
	bool check_fail_max_spd_err;
	bool check_fail_max_horz_drift;
	bool check_fail_max_vert_drift;
	bool check_fail_max_horz_spd_err;
	bool check_fail_max_vert_spd_err;
	uint8_t _padding0[1]; // required for logger


#ifdef __cplusplus

#endif
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using EstimatorGpsStatus = estimator_gps_status_s;
	} // namespace msg
} // namespace nextpilot
#endif

/* register this as object request broker structure */
ORB_DECLARE(estimator_gps_status);


#ifdef __cplusplus
void print_message(const orb_metadata *meta, const estimator_gps_status_s& message);
#endif