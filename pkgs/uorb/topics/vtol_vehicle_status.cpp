
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file VtolVehicleStatus.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/vtol_vehicle_status.h>

constexpr char __orb_vtol_vehicle_status_fields[] = "\x89 timestamp;\x86 vehicle_vtol_state;\x8c fixed_wing_system_failure;\x86[6] _padding0;";

ORB_DEFINE(vtol_vehicle_status, struct vtol_vehicle_status_s, 10, __orb_vtol_vehicle_status_fields, static_cast<uint8_t>(ORB_ID::vtol_vehicle_status));


void print_message(const orb_metadata *meta, const vtol_vehicle_status_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}