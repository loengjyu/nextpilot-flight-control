
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file TelemetryStatus.msg */


#include <rtdbg.h>
#include <inttypes.h>
#include <hrtimer.h>
#include <topics/uORBTopics.hpp>
#include <topics/telemetry_status.h>

constexpr char __orb_telemetry_status_fields[] = "\x89 timestamp;\x8a data_rate;\x8a rate_multiplier;\x8a tx_rate_avg;\x8a tx_error_rate_avg;\x88 tx_message_count;\x88 tx_buffer_overruns;\x8a rx_rate_avg;\x88 rx_message_count;\x88 rx_message_lost_count;\x88 rx_buffer_overruns;\x88 rx_parse_errors;\x88 rx_packet_drop_count;\x8a rx_message_lost_rate;\x86 type;\x86 mode;\x8c flow_control;\x8c forwarding;\x8c mavlink_v2;\x8c ftp;\x86 streams;\x8c heartbeat_type_antenna_tracker;\x8c heartbeat_type_gcs;\x8c heartbeat_type_onboard_controller;\x8c heartbeat_type_gimbal;\x8c heartbeat_type_adsb;\x8c heartbeat_type_camera;\x8c heartbeat_type_parachute;\x8c heartbeat_type_open_drone_id;\x8c heartbeat_component_telemetry_radio;\x8c heartbeat_component_log;\x8c heartbeat_component_osd;\x8c heartbeat_component_obstacle_avoidance;\x8c heartbeat_component_vio;\x8c heartbeat_component_pairing_manager;\x8c heartbeat_component_udp_bridge;\x8c heartbeat_component_uart_bridge;\x8c avoidance_system_healthy;\x8c open_drone_id_system_healthy;\x8c parachute_system_healthy;\x86[2] _padding0;";

ORB_DEFINE(telemetry_status, struct telemetry_status_s, 86, __orb_telemetry_status_fields, static_cast<uint8_t>(ORB_ID::telemetry_status));


void print_message(const orb_metadata *meta, const telemetry_status_s& message)
{
	if (sizeof(message) != meta->o_size) {
		LOG_RAW("unexpected message size for %s: %zu != %i\n", meta->o_name, sizeof(message), meta->o_size);
		return;
	}
	// orb_print_message_internal(meta, &message, true);
}