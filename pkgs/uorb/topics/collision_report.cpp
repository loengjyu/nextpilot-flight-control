
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file CollisionReport.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/collision_report.h>

constexpr char __orb_collision_report_fields[] = "\x89 timestamp;\x88 id;\x8a time_to_minimum_delta;\x8a altitude_minimum_delta;\x8a horizontal_minimum_delta;\x86 src;\x86 action;\x86 threat_level;\x86[5] _padding0;";

ORB_DEFINE(collision_report, struct collision_report_s, 27, __orb_collision_report_fields, static_cast<uint8_t>(ORB_ID::collision_report));


void print_message(const orb_metadata *meta, const collision_report_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}