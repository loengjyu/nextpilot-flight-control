
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file CameraCapture.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/camera_capture.h>

constexpr char __orb_camera_capture_fields[] = "\x89 timestamp;\x89 timestamp_utc;\x8b lat;\x8b lon;\x88 seq;\x8a alt;\x8a ground_distance;\x8a[4] q;\x82 result;\x86[3] _padding0;";

ORB_DEFINE(camera_capture, struct camera_capture_s, 61, __orb_camera_capture_fields, static_cast<uint8_t>(ORB_ID::camera_capture));


void print_message(const orb_metadata *meta, const camera_capture_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}