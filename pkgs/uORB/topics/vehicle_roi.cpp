
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file VehicleRoi.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/vehicle_roi.h>

constexpr char __orb_vehicle_roi_fields[] = "\x89 timestamp;\x8b lat;\x8b lon;\x8a alt;\x8a roll_offset;\x8a pitch_offset;\x8a yaw_offset;\x86 mode;\x86[7] _padding0;";

ORB_DEFINE(vehicle_roi, struct vehicle_roi_s, 41, __orb_vehicle_roi_fields, static_cast<uint8_t>(ORB_ID::vehicle_roi));


void print_message(const orb_metadata *meta, const vehicle_roi_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}