
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file TransponderReport.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/transponder_report.h>

constexpr char __orb_transponder_report_fields[] = "\x89 timestamp;\x8b lat;\x8b lon;\x88 icao_address;\x8a altitude;\x8a heading;\x8a hor_velocity;\x8a ver_velocity;\x87 flags;\x87 squawk;\x86 altitude_type;\x8d[9] callsign;\x86 emitter_type;\x86 tslc;\x86[18] uas_id;\x86[2] _padding0;";

ORB_DEFINE(transponder_report, struct transponder_report_s, 78, __orb_transponder_report_fields, static_cast<uint8_t>(ORB_ID::transponder_report));


void print_message(const orb_metadata *meta, const transponder_report_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}