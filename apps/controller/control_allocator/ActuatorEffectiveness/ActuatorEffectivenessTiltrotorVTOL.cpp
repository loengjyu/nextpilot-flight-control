/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/**
 * @file ActuatorEffectivenessTiltrotorVTOL.hpp
 *
 * Actuator effectiveness for tiltrotor VTOL
 *
 * @author Julien Lecoeur <julien.lecoeur@gmail.com>
 */

#include "ActuatorEffectivenessTiltrotorVTOL.hpp"

using namespace matrix;

ActuatorEffectivenessTiltrotorVTOL::ActuatorEffectivenessTiltrotorVTOL(ModuleParams *parent) :
    ModuleParams(parent),
    _mc_rotors(this, ActuatorEffectivenessRotors::AxisConfiguration::Configurable, true),
    _control_surfaces(this), _tilts(this) {
    setFlightPhase(FlightPhase::HOVER_FLIGHT);
}

bool ActuatorEffectivenessTiltrotorVTOL::getEffectivenessMatrix(Configuration            &configuration,
                                                                EffectivenessUpdateReason external_update) {
    if (!_collective_tilt_updated && external_update == EffectivenessUpdateReason::NO_EXTERNAL_UPDATE) {
        return false;
    }

    // MC motors
    configuration.selected_matrix = 0;
    _mc_rotors.enableYawByDifferentialThrust(!_tilts.hasYawControl());
    _mc_rotors.enableThreeDimensionalThrust(false);

    // Update matrix with tilts in vertical position when update is triggered by a manual
    // configuration (parameter) change. This is to make sure the normalization
    // scales are tilt-invariant. Note: configuration updates are only possible when disarmed.
    const float collective_tilt_control_applied = (external_update == EffectivenessUpdateReason::CONFIGURATION_UPDATE) ?
                                                      -1.f :
                                                      _last_collective_tilt_control;
    _untiltable_motors                          = _mc_rotors.updateAxisFromTilts(_tilts, collective_tilt_control_applied)
                         << configuration.num_actuators[(int)ActuatorType::MOTORS];

    const bool mc_rotors_added_successfully = _mc_rotors.addActuators(configuration);
    _motors                                 = _mc_rotors.getMotors();

    // Control Surfaces
    configuration.selected_matrix          = 1;
    _first_control_surface_idx             = configuration.num_actuators_matrix[configuration.selected_matrix];
    const bool surfaces_added_successfully = _control_surfaces.addActuators(configuration);

    // Tilts
    configuration.selected_matrix = 0;
    _first_tilt_idx               = configuration.num_actuators_matrix[configuration.selected_matrix];
    _tilts.updateTorqueSign(_mc_rotors.geometry(), true /* disable pitch to avoid configuration errors */);
    const bool tilts_added_successfully = _tilts.addActuators(configuration);

    // If it was an update coming from a config change, then make sure to update matrix in
    // the next iteration again with the correct tilt (but without updating the normalization scale).
    _collective_tilt_updated = (external_update == EffectivenessUpdateReason::CONFIGURATION_UPDATE);

    return (mc_rotors_added_successfully && surfaces_added_successfully && tilts_added_successfully);
}

void ActuatorEffectivenessTiltrotorVTOL::allocateAuxilaryControls(const float dt, int matrix_index,
                                                                  ActuatorVector &actuator_sp) {
    if (matrix_index == 1) {
        // apply flaps
        normalized_unsigned_setpoint_s flaps_setpoint;

        if (_flaps_setpoint_sub.copy(&flaps_setpoint)) {
            _control_surfaces.applyFlaps(flaps_setpoint.normalized_setpoint, _first_control_surface_idx, dt, actuator_sp);
        }

        // apply spoilers
        normalized_unsigned_setpoint_s spoilers_setpoint;

        if (_spoilers_setpoint_sub.copy(&spoilers_setpoint)) {
            _control_surfaces.applySpoilers(spoilers_setpoint.normalized_setpoint, _first_control_surface_idx, dt, actuator_sp);
        }
    }
}

void ActuatorEffectivenessTiltrotorVTOL::updateSetpoint(const matrix::Vector<float, NUM_AXES> &control_sp,
                                                        int matrix_index, ActuatorVector &actuator_sp, const matrix::Vector<float, NUM_ACTUATORS> &actuator_min,
                                                        const matrix::Vector<float, NUM_ACTUATORS> &actuator_max) {
    // apply tilt
    if (matrix_index == 0) {
        tiltrotor_extra_controls_s tiltrotor_extra_controls;

        if (_tiltrotor_extra_controls_sub.copy(&tiltrotor_extra_controls)) {
            float control_collective_tilt = tiltrotor_extra_controls.collective_tilt_normalized_setpoint * 2.f - 1.f;

            // set control_collective_tilt to exactly -1 or 1 if close to these end points
            control_collective_tilt = control_collective_tilt < -0.99f ? -1.f : control_collective_tilt;
            control_collective_tilt = control_collective_tilt > 0.99f ? 1.f : control_collective_tilt;

            // initialize _last_collective_tilt_control
            if (!PX4_ISFINITE(_last_collective_tilt_control)) {
                _last_collective_tilt_control = control_collective_tilt;

            } else if (fabsf(control_collective_tilt - _last_collective_tilt_control) > 0.01f) {
                _collective_tilt_updated      = true;
                _last_collective_tilt_control = control_collective_tilt;
            }

            // During transition to FF, only allow update thrust axis up to 45° as with a high tilt angle the effectiveness
            // of the thrust axis in z is apporaching 0, and by that is increasing the motor output to max.
            // Transition to HF: disable thrust axis tilting, and assume motors are vertical. This is to avoid
            // a thrust spike when the transition is initiated (as then the tilt is fully forward).
            if (_flight_phase == FlightPhase::TRANSITION_HF_TO_FF) {
                _last_collective_tilt_control = math::constrain(_last_collective_tilt_control, -1.f, 0.f);

            } else if (_flight_phase == FlightPhase::TRANSITION_FF_TO_HF) {
                _last_collective_tilt_control = -1.f;
            }

            for (int i = 0; i < _tilts.count(); ++i) {
                if (_tilts.config(i).tilt_direction == ActuatorEffectivenessTilts::TiltDirection::TowardsFront) {
                    actuator_sp(i + _first_tilt_idx) += control_collective_tilt;
                }
            }

            // in FW directly use throttle sp
            if (_flight_phase == FlightPhase::FORWARD_FLIGHT) {
                for (int i = 0; i < _first_tilt_idx; ++i) {
                    actuator_sp(i) = tiltrotor_extra_controls.collective_thrust_normalized_setpoint;
                }
            }
        }

        if (_flight_phase == FlightPhase::FORWARD_FLIGHT) {
            stopMaskedMotorsWithZeroThrust(_motors & ~_untiltable_motors, actuator_sp);
        }
    }

    // Set yaw saturation flag in case of yaw through tilt. As in this case the yaw actuation is decoupled from
    // the other axes (for now neglecting the case of 0 collective thrust), we set the saturation flags
    // directly if the (normalized) yaw torque setpoint is outside of range (-1, 1).
    if (matrix_index == 0 && _tilts.hasYawControl()) {
        _yaw_tilt_saturation_flags.tilt_yaw_neg = false;
        _yaw_tilt_saturation_flags.tilt_yaw_pos = false;

        if (control_sp(2) < -1.f) {
            _yaw_tilt_saturation_flags.tilt_yaw_neg = true;

        } else if (control_sp(2) > 1.f) {
            _yaw_tilt_saturation_flags.tilt_yaw_pos = true;
        }
    }
}

void ActuatorEffectivenessTiltrotorVTOL::setFlightPhase(const FlightPhase &flight_phase) {
    if (_flight_phase == flight_phase) {
        return;
    }

    ActuatorEffectiveness::setFlightPhase(flight_phase);

    // update stopped motors
    switch (flight_phase) {
    case FlightPhase::FORWARD_FLIGHT:
        _stopped_motors_mask |= _untiltable_motors;
        break;

    case FlightPhase::HOVER_FLIGHT:
    case FlightPhase::TRANSITION_FF_TO_HF:
    case FlightPhase::TRANSITION_HF_TO_FF:
        _stopped_motors_mask = 0;
        break;
    }
}

void ActuatorEffectivenessTiltrotorVTOL::getUnallocatedControl(int matrix_index, control_allocator_status_s &status) {
    // only handle matrix 0 (motors and tilts)
    if (matrix_index == 1) {
        return;
    }

    // Note: the values '-1', '1' and '0' are just to indicate a negative,
    // positive or no saturation to the rate controller. The actual magnitude is not used.
    if (_yaw_tilt_saturation_flags.tilt_yaw_pos) {
        status.unallocated_torque[2] = 1.f;

    } else if (_yaw_tilt_saturation_flags.tilt_yaw_neg) {
        status.unallocated_torque[2] = -1.f;

    } else {
        status.unallocated_torque[2] = 0.f;
    }
}
