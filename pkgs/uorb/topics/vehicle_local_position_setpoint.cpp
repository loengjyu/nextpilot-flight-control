
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file VehicleLocalPositionSetpoint.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/vehicle_local_position_setpoint.h>

constexpr char __orb_vehicle_local_position_setpoint_fields[] = "\x89 timestamp;\x8a x;\x8a y;\x8a z;\x8a vx;\x8a vy;\x8a vz;\x8a[3] acceleration;\x8a[3] thrust;\x8a yaw;\x8a yawspeed;";

ORB_DEFINE(vehicle_local_position_setpoint, struct vehicle_local_position_setpoint_s, 64, __orb_vehicle_local_position_setpoint_fields, static_cast<uint8_t>(ORB_ID::vehicle_local_position_setpoint));


void print_message(const orb_metadata *meta, const vehicle_local_position_setpoint_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}