
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file OrbitStatus.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#define ORBIT_STATUS_ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TO_CIRCLE_CENTER 0
#define ORBIT_STATUS_ORBIT_YAW_BEHAVIOUR_HOLD_INITIAL_HEADING 1
#define ORBIT_STATUS_ORBIT_YAW_BEHAVIOUR_UNCONTROLLED 2
#define ORBIT_STATUS_ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TANGENT_TO_CIRCLE 3
#define ORBIT_STATUS_ORBIT_YAW_BEHAVIOUR_RC_CONTROLLED 4
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT orbit_status_s {
#else
struct orbit_status_s {
#endif // __cplusplus
	uint64_t timestamp;
	double x;
	double y;
	float radius;
	float z;
	uint8_t frame;
	uint8_t yaw_behaviour;
	uint8_t _padding0[6]; // required for logger

#ifdef __cplusplus
	static constexpr uint8_t ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TO_CIRCLE_CENTER = 0;
	static constexpr uint8_t ORBIT_YAW_BEHAVIOUR_HOLD_INITIAL_HEADING = 1;
	static constexpr uint8_t ORBIT_YAW_BEHAVIOUR_UNCONTROLLED = 2;
	static constexpr uint8_t ORBIT_YAW_BEHAVIOUR_HOLD_FRONT_TANGENT_TO_CIRCLE = 3;
	static constexpr uint8_t ORBIT_YAW_BEHAVIOUR_RC_CONTROLLED = 4;
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using OrbitStatus = orbit_status_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(orbit_status);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const orbit_status_s& message);
#endif // __cplusplus