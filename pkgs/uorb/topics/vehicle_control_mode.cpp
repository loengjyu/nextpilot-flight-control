
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file VehicleControlMode.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/vehicle_control_mode.h>

constexpr char __orb_vehicle_control_mode_fields[] = "\x89 timestamp;\x8c flag_armed;\x8c flag_multicopter_position_control_enabled;\x8c flag_control_manual_enabled;\x8c flag_control_auto_enabled;\x8c flag_control_offboard_enabled;\x8c flag_control_rates_enabled;\x8c flag_control_attitude_enabled;\x8c flag_control_acceleration_enabled;\x8c flag_control_velocity_enabled;\x8c flag_control_position_enabled;\x8c flag_control_altitude_enabled;\x8c flag_control_climb_rate_enabled;\x8c flag_control_termination_enabled;\x86[3] _padding0;";

ORB_DEFINE(vehicle_control_mode, struct vehicle_control_mode_s, 21, __orb_vehicle_control_mode_fields, static_cast<uint8_t>(ORB_ID::vehicle_control_mode));


void print_message(const orb_metadata *meta, const vehicle_control_mode_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}