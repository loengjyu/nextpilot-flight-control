
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file FollowTargetEstimator.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/follow_target_estimator.h>

constexpr char __orb_follow_target_estimator_fields[] = "\x89 timestamp;\x89 last_filter_reset_timestamp;\x8b lat_est;\x8b lon_est;\x89 prediction_count;\x89 fusion_count;\x8a alt_est;\x8a[3] pos_est;\x8a[3] vel_est;\x8a[3] acc_est;\x8c valid;\x8c stale;\x86[6] _padding0;";

ORB_DEFINE(follow_target_estimator, struct follow_target_estimator_s, 90, __orb_follow_target_estimator_fields, static_cast<uint8_t>(ORB_ID::follow_target_estimator));


void print_message(const orb_metadata *meta, const follow_target_estimator_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}