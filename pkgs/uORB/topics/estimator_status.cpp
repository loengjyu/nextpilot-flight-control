
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file EstimatorStatus.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/estimator_status.h>

constexpr char __orb_estimator_status_fields[] = "\x89 timestamp;\x89 timestamp_sample;\x89 control_mode_flags;\x8a[3] output_tracking_error;\x88 filter_fault_flags;\x8a pos_horiz_accuracy;\x8a pos_vert_accuracy;\x8a mag_test_ratio;\x8a vel_test_ratio;\x8a pos_test_ratio;\x8a hgt_test_ratio;\x8a tas_test_ratio;\x8a hagl_test_ratio;\x8a beta_test_ratio;\x8a time_slip;\x88 accel_device_id;\x88 gyro_device_id;\x88 baro_device_id;\x88 mag_device_id;\x87 gps_check_fail_flags;\x87 innovation_check_flags;\x87 solution_status_flags;\x86 reset_count_vel_ne;\x86 reset_count_vel_d;\x86 reset_count_pos_ne;\x86 reset_count_pod_d;\x86 reset_count_quat;\x8c pre_flt_fail_innov_heading;\x8c pre_flt_fail_innov_vel_horiz;\x8c pre_flt_fail_innov_vel_vert;\x8c pre_flt_fail_innov_height;\x8c pre_flt_fail_mag_field_disturbed;\x86 health_flags;\x86 timeout_flags;\x86[6] _padding0;";

ORB_DEFINE(estimator_status, struct estimator_status_s, 114, __orb_estimator_status_fields, static_cast<uint8_t>(ORB_ID::estimator_status));


void print_message(const orb_metadata *meta, const estimator_status_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}