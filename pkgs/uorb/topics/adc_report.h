
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


#pragma once


#include <uORB.h>


#ifndef __cplusplus

#endif


#ifdef __cplusplus
struct __EXPORT adc_report_s {
#else
struct adc_report_s {
#endif
	uint64_t timestamp;
	uint32_t device_id;
	int32_t raw_data[12];
	uint32_t resolution;
	float v_ref;
	int16_t channel_id[12];
	uint8_t _padding0[4]; // required for logger


#ifdef __cplusplus

#endif
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using AdcReport = adc_report_s;
	} // namespace msg
} // namespace nextpilot
#endif

/* register this as object request broker structure */
ORB_DECLARE(adc_report);


#ifdef __cplusplus
void print_message(const orb_metadata *meta, const adc_report_s& message);
#endif