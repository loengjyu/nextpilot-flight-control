
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file RcParameterMap.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#define RC_PARAMETER_MAP_RC_PARAM_MAP_NCHAN 3
#define RC_PARAMETER_MAP_PARAM_ID_LEN 16
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT rc_parameter_map_s {
#else
struct rc_parameter_map_s {
#endif // __cplusplus
	uint64_t timestamp;
	int32_t param_index[3];
	float scale[3];
	float value0[3];
	float value_min[3];
	float value_max[3];
	bool valid[3];
	char param_id[51];
	uint8_t _padding0[6]; // required for logger

#ifdef __cplusplus
	static constexpr uint8_t RC_PARAM_MAP_NCHAN = 3;
	static constexpr uint8_t PARAM_ID_LEN = 16;
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using RcParameterMap = rc_parameter_map_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(rc_parameter_map);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const rc_parameter_map_s& message);
#endif // __cplusplus