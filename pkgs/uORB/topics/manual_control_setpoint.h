
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file ManualControlSetpoint.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#define MANUAL_CONTROL_SETPOINT_SOURCE_UNKNOWN 0
#define MANUAL_CONTROL_SETPOINT_SOURCE_RC 1
#define MANUAL_CONTROL_SETPOINT_SOURCE_MAVLINK_0 2
#define MANUAL_CONTROL_SETPOINT_SOURCE_MAVLINK_1 3
#define MANUAL_CONTROL_SETPOINT_SOURCE_MAVLINK_2 4
#define MANUAL_CONTROL_SETPOINT_SOURCE_MAVLINK_3 5
#define MANUAL_CONTROL_SETPOINT_SOURCE_MAVLINK_4 6
#define MANUAL_CONTROL_SETPOINT_SOURCE_MAVLINK_5 7
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT manual_control_setpoint_s {
#else
struct manual_control_setpoint_s {
#endif // __cplusplus
	uint64_t timestamp;
	uint64_t timestamp_sample;
	float roll;
	float pitch;
	float yaw;
	float throttle;
	float flaps;
	float aux1;
	float aux2;
	float aux3;
	float aux4;
	float aux5;
	float aux6;
	bool valid;
	uint8_t data_source;
	bool sticks_moving;
	uint8_t _padding0[1]; // required for logger

#ifdef __cplusplus
	static constexpr uint8_t SOURCE_UNKNOWN = 0;
	static constexpr uint8_t SOURCE_RC = 1;
	static constexpr uint8_t SOURCE_MAVLINK_0 = 2;
	static constexpr uint8_t SOURCE_MAVLINK_1 = 3;
	static constexpr uint8_t SOURCE_MAVLINK_2 = 4;
	static constexpr uint8_t SOURCE_MAVLINK_3 = 5;
	static constexpr uint8_t SOURCE_MAVLINK_4 = 6;
	static constexpr uint8_t SOURCE_MAVLINK_5 = 7;
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using ManualControlSetpoint = manual_control_setpoint_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(manual_control_setpoint);
ORB_DECLARE(manual_control_input);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const manual_control_setpoint_s& message);
#endif // __cplusplus