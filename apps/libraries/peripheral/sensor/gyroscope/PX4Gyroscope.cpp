/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/


#include "PX4Gyroscope.hpp"
#include <device/device_id.h>
#include <param/param.h>

using namespace time_literals;

static constexpr int32_t sum(const int16_t samples[], uint8_t len)
{
	int32_t sum = 0;

	for (int n = 0; n < len; n++) {
		sum += samples[n];
	}

	return sum;
}

static constexpr uint8_t clipping(const int16_t samples[], uint8_t len)
{
	unsigned clip_count = 0;

	for (int n = 0; n < len; n++) {
		// - consider data clipped/saturated if it's INT16_MIN/INT16_MAX or within 1
		// - this accommodates rotated data (|INT16_MIN| = INT16_MAX + 1)
		//   and sensors that may re-use the lowest bit for other purposes (sync indicator, etc)
		if ((samples[n] <= INT16_MIN + 1) || (samples[n] >= INT16_MAX - 1)) {
			clip_count++;
		}
	}

	return clip_count;
}

PX4Gyroscope::PX4Gyroscope(uint32_t device_id, enum Rotation rotation) :
	_device_id{device_id},
	_rotation{rotation}
{
	// advertise immediately to keep instance numbering in sync
	_sensor_pub.advertise();

	param_get(param_find("IMU_GYRO_RATEMAX"), &_imu_gyro_rate_max);
}

PX4Gyroscope::~PX4Gyroscope()
{
	_sensor_pub.unadvertise();
	_sensor_fifo_pub.unadvertise();
}

void PX4Gyroscope::set_device_type(uint8_t devtype)
{
	// current DeviceStructure
	union device::DeviceId device_id;
	device_id.devid = _device_id;

	// update to new device type
	device_id.devid_s.devtype = devtype;

	// copy back
	_device_id = device_id.devid;
}

void PX4Gyroscope::set_scale(float scale)
{
	if (fabsf(scale - _scale) > FLT_EPSILON) {
		// rescale last sample on scale change
		float rescale = _scale / scale;

		for (auto &s : _last_sample) {
			s = roundf(s * rescale);
		}

		_scale = scale;

		UpdateClipLimit();
	}
}

void PX4Gyroscope::update(const hrt_abstime &timestamp_sample, float x, float y, float z)
{
	// Apply rotation (before scaling)
	rotate_3f(_rotation, x, y, z);

	sensor_gyro_s report;

	report.timestamp_sample = timestamp_sample;
	report.device_id = _device_id;
	report.temperature = _temperature;
	report.error_count = _error_count;
	report.x = x * _scale;
	report.y = y * _scale;
	report.z = z * _scale;
	report.clip_counter[0] = (fabsf(x) >= _clip_limit);
	report.clip_counter[1] = (fabsf(y) >= _clip_limit);
	report.clip_counter[2] = (fabsf(z) >= _clip_limit);
	report.samples = 1;
	report.timestamp = hrt_absolute_time();

	_sensor_pub.publish(report);
}

void PX4Gyroscope::updateFIFO(sensor_gyro_fifo_s &sample)
{
	// rotate all raw samples and publish fifo
	const uint8_t N = sample.samples;

	for (int n = 0; n < N; n++) {
		rotate_3i(_rotation, sample.x[n], sample.y[n], sample.z[n]);
	}

	sample.device_id = _device_id;
	sample.scale = _scale;
	sample.timestamp = hrt_absolute_time();
	_sensor_fifo_pub.publish(sample);


	// publish
	sensor_gyro_s report;
	report.timestamp_sample = sample.timestamp_sample;
	report.device_id = _device_id;
	report.temperature = _temperature;
	report.error_count = _error_count;

	// trapezoidal integration (equally spaced)
	const float scale = _scale / (float)N;
	report.x = (0.5f * (_last_sample[0] + sample.x[N - 1]) + sum(sample.x, N - 1)) * scale;
	report.y = (0.5f * (_last_sample[1] + sample.y[N - 1]) + sum(sample.y, N - 1)) * scale;
	report.z = (0.5f * (_last_sample[2] + sample.z[N - 1]) + sum(sample.z, N - 1)) * scale;

	_last_sample[0] = sample.x[N - 1];
	_last_sample[1] = sample.y[N - 1];
	_last_sample[2] = sample.z[N - 1];

	report.clip_counter[0] = clipping(sample.x, N);
	report.clip_counter[1] = clipping(sample.y, N);
	report.clip_counter[2] = clipping(sample.z, N);
	report.samples = N;
	report.timestamp = hrt_absolute_time();

	_sensor_pub.publish(report);
}

void PX4Gyroscope::UpdateClipLimit()
{
	// 99.9% of potential max
	_clip_limit = math::constrain((_range / _scale) * 0.999f, 0.f, (float)INT16_MAX);
}
