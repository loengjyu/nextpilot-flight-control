
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file HealthReport.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/health_report.h>

constexpr char __orb_health_report_fields[] = "\x89 timestamp;\x89 can_arm_mode_flags;\x89 can_run_mode_flags;\x89 health_is_present_flags;\x89 health_warning_flags;\x89 health_error_flags;\x89 arming_check_warning_flags;\x89 arming_check_error_flags;";

ORB_DEFINE(health_report, struct health_report_s, 64, __orb_health_report_fields, static_cast<uint8_t>(ORB_ID::health_report));


void print_message(const orb_metadata *meta, const health_report_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}