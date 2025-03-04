
/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/* Auto-generated by genmsg_cpp from file SensorGyroFft.msg */

#pragma once

#include <uORB.h>

#ifndef __cplusplus
#endif // __cplusplus


#ifdef __cplusplus
struct __EXPORT sensor_gyro_fft_s {
#else
struct sensor_gyro_fft_s {
#endif // __cplusplus
	uint64_t timestamp;
	uint64_t timestamp_sample;
	uint32_t device_id;
	float sensor_sample_rate_hz;
	float resolution_hz;
	float peak_frequencies_x[3];
	float peak_frequencies_y[3];
	float peak_frequencies_z[3];
	float peak_snr_x[3];
	float peak_snr_y[3];
	float peak_snr_z[3];
	uint8_t _padding0[4]; // required for logger

#ifdef __cplusplus
#endif // __cplusplus
};

#ifdef __cplusplus
namespace nextpilot {
	namespace msg {
		using SensorGyroFft = sensor_gyro_fft_s;
	} // namespace msg
} // namespace nextpilot
#endif // __cplusplus

/* register this as object request broker structure */
ORB_DECLARE(sensor_gyro_fft);

#ifdef __cplusplus
void print_message(const orb_metadata *meta, const sensor_gyro_fft_s& message);
#endif // __cplusplus
