
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file AdcReport.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/adc_report.h>

constexpr char __orb_adc_report_fields[] = "\x89 timestamp;\x88 device_id;\x84[12] raw_data;\x88 resolution;\x8a v_ref;\x83[12] channel_id;\x86[4] _padding0;";

ORB_DEFINE(adc_report, struct adc_report_s, 92, __orb_adc_report_fields, static_cast<uint8_t>(ORB_ID::adc_report));


void print_message(const orb_metadata *meta, const adc_report_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}