
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file ActuatorTest.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#define ACTUATOR_TEST_ACTION_RELEASE_CONTROL 0
#define ACTUATOR_TEST_ACTION_DO_CONTROL 1
#define ACTUATOR_TEST_FUNCTION_MOTOR1 101
#define ACTUATOR_TEST_MAX_NUM_MOTORS 12
#define ACTUATOR_TEST_FUNCTION_SERVO1 201
#define ACTUATOR_TEST_MAX_NUM_SERVOS 8
#define ACTUATOR_TEST_ORB_QUEUE_LENGTH 12
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT actuator_test_s {
#else
struct actuator_test_s {
#endif // __cplusplus
	uint64_t timestamp;
	float value;
	uint32_t timeout_ms;
	uint16_t function;
	uint8_t action;
	uint8_t _padding0[5]; // required for logger

#ifdef __cplusplus
	static constexpr uint8_t ACTION_RELEASE_CONTROL = 0;
	static constexpr uint8_t ACTION_DO_CONTROL = 1;
	static constexpr uint8_t FUNCTION_MOTOR1 = 101;
	static constexpr uint8_t MAX_NUM_MOTORS = 12;
	static constexpr uint8_t FUNCTION_SERVO1 = 201;
	static constexpr uint8_t MAX_NUM_SERVOS = 8;
	static constexpr uint8_t ORB_QUEUE_LENGTH = 12;
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using ActuatorTest = actuator_test_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(actuator_test);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const actuator_test_s& message);
#endif // __cplusplus
