
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file VehicleAirData.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/vehicle_air_data.h>

constexpr char __orb_vehicle_air_data_fields[] = "\x89 timestamp;\x89 timestamp_sample;\x88 baro_device_id;\x8a baro_alt_meter;\x8a baro_temp_celcius;\x8a baro_pressure_pa;\x8a rho;\x86 calibration_count;\x86[3] _padding0;";

ORB_DEFINE(vehicle_air_data, struct vehicle_air_data_s, 37, __orb_vehicle_air_data_fields, static_cast<uint8_t>(ORB_ID::vehicle_air_data));


void print_message(const orb_metadata *meta, const vehicle_air_data_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}