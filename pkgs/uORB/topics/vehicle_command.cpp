
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file VehicleCommand.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/vehicle_command.h>

constexpr char __orb_vehicle_command_fields[] = "\x89 timestamp;\x8b param5;\x8b param6;\x8a param1;\x8a param2;\x8a param3;\x8a param4;\x8a param7;\x88 command;\x86 target_system;\x86 target_component;\x86 source_system;\x86 source_component;\x86 confirmation;\x8c from_external;\x86[2] _padding0;";

ORB_DEFINE(vehicle_command, struct vehicle_command_s, 54, __orb_vehicle_command_fields, static_cast<uint8_t>(ORB_ID::vehicle_command));
ORB_DEFINE(gimbal_v1_command, struct vehicle_command_s, 54, __orb_vehicle_command_fields, static_cast<uint8_t>(ORB_ID::gimbal_v1_command));


void print_message(const orb_metadata *meta, const vehicle_command_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}