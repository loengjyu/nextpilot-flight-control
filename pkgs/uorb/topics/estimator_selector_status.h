
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file EstimatorSelectorStatus.msg */


#pragma once


#include <uORB.h>


#ifndef __cplusplus

#endif


#ifdef __cplusplus
struct __EXPORT estimator_selector_status_s {
#else
struct estimator_selector_status_s {
#endif
	uint64_t timestamp;
	uint64_t last_instance_change;
	uint32_t instance_changed_count;
	uint32_t accel_device_id;
	uint32_t baro_device_id;
	uint32_t gyro_device_id;
	uint32_t mag_device_id;
	float combined_test_ratio[9];
	float relative_test_ratio[9];
	float accumulated_gyro_error[4];
	float accumulated_accel_error[4];
	uint8_t primary_instance;
	uint8_t instances_available;
	bool healthy[9];
	bool gyro_fault_detected;
	bool accel_fault_detected;
	uint8_t _padding0[7]; // required for logger


#ifdef __cplusplus

#endif
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using EstimatorSelectorStatus = estimator_selector_status_s;
	} // namespace msg
} // namespace nextpilot
#endif

/* register this as object request broker structure */
ORB_DECLARE(estimator_selector_status);


#ifdef __cplusplus
void print_message(const orb_metadata *meta, const estimator_selector_status_s& message);
#endif