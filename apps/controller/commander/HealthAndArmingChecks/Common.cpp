/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

#define LOG_TAG "cmder.common"
#define LOG_LVL LOG_LVL_INFO

#include "Common.hpp"
#include "../ModeUtil/mode_requirements.hpp"

void Report::getHealthReport(health_report_s &report) const {
    const Results &current_results = _results[_current_result];
    report.can_arm_mode_flags      = 0;
    report.can_run_mode_flags      = 0;

    for (int i = 0; i < vehicle_status_s::NAVIGATION_STATE_MAX; ++i) {
        NavModes group = getModeGroup(i);

        if ((uint32_t)(current_results.arming_checks.can_arm & group)) {
            report.can_arm_mode_flags |= 1ull << i;
        }

        if ((uint32_t)(current_results.arming_checks.can_run & group)) {
            report.can_run_mode_flags |= 1ull << i;
        }
    }

    report.arming_check_error_flags   = (uint64_t)current_results.arming_checks.error;
    report.arming_check_warning_flags = (uint64_t)current_results.arming_checks.warning;
    report.health_is_present_flags    = (uint64_t)current_results.health.is_present;
    report.health_error_flags         = (uint64_t)current_results.health.error;
    report.health_warning_flags       = (uint64_t)current_results.health.warning;
}

void Report::healthFailure(NavModes required_modes, HealthComponentIndex component, uint32_t event_id,
                           const events::LogLevels &log_levels, const char *message) {
    healthFailure(required_modes, component, log_levels.external);
    addEvent(event_id, log_levels, message, (uint32_t)reportedModes(required_modes), component.index);
}

void Report::armingCheckFailure(NavModes required_modes, HealthComponentIndex component, uint32_t event_id,
                                const events::LogLevels &log_levels, const char *message) {
    armingCheckFailure(required_modes, component, log_levels.external);
    addEvent(event_id, log_levels, message, (uint32_t)reportedModes(required_modes), component.index);
}

Report::EventBufferHeader *Report::addEventToBuffer(uint32_t event_id, const events::LogLevels &log_levels,
                                                    uint32_t modes, unsigned args_size) {
    unsigned           total_size = sizeof(EventBufferHeader) + args_size;
    EventBufferHeader *header     = (EventBufferHeader *)(_event_buffer + _next_buffer_idx);
    memcpy(&header->id, &event_id, sizeof(event_id)); // header might be unaligned
    header->log_levels  = ((uint8_t)log_levels.internal << 4) | (uint8_t)log_levels.external;
    header->size        = args_size;
    _next_buffer_idx   += total_size;
    ++_results[_current_result].num_events;
    _results[_current_result].event_id_hash ^= event_id ^ modes; // very simple hash
    return header;
}

NavModes Report::reportedModes(NavModes required_modes) {
    // Make sure optional checks are still shown in the UI
    if (required_modes == NavModes::None) {
        return NavModes::All;
    }

    return required_modes;
}

void Report::setIsPresent(health_component_t component) {
    HealthResults &health = _results[_current_result].health;
    health.is_present     = health.is_present | component;
}

void Report::setHealth(health_component_t component, bool is_present, bool warning, bool error) {
    HealthResults &health = _results[_current_result].health;

    if (is_present) {
        health.is_present = health.is_present | component;
    }

    if (warning) {
        health.warning = health.warning | component;
    }

    if (error) {
        health.error = health.error | component;
    }
}

void Report::healthFailure(NavModes required_modes, HealthComponentIndex component, events::Log log_level) {
    // update current health results
    HealthResults &health = _results[_current_result].health;

    if (log_level <= events::Log::Error) {
        health.error = health.error | (events::px4::enums::health_component_t)(1 << component.index);

    } else if (log_level <= events::Log::Warning) {
        health.warning = health.warning | (events::px4::enums::health_component_t)(1 << component.index);
    }

    // clear relevant arming bits
    clearArmingBits(required_modes);
}

void Report::armingCheckFailure(NavModes required_modes, HealthComponentIndex component, events::Log log_level) {
    // update current arming check results
    ArmingCheckResults &arming_checks = _results[_current_result].arming_checks;

    if (log_level <= events::Log::Error) {
        arming_checks.error = arming_checks.error | (events::px4::enums::health_component_t)(1 << component.index);

    } else if (log_level <= events::Log::Warning) {
        arming_checks.warning = arming_checks.warning | (events::px4::enums::health_component_t)(1 << component.index);
    }

    // clear relevant arming bits
    clearArmingBits(required_modes);
}

void Report::clearArmingBits(NavModes modes) {
    ArmingCheckResults &arming_checks = _results[_current_result].arming_checks;
    NavModes           &can_arm       = arming_checks.can_arm;
    can_arm                           = can_arm & ~modes;
}

void Report::clearCanRunBits(NavModes modes) {
    ArmingCheckResults &arming_checks = _results[_current_result].arming_checks;
    NavModes           &can_run       = arming_checks.can_run;
    can_run                           = can_run & ~modes;
}

void Report::reset() {
    _current_result = (_current_result + 1) % 2;
    _results[_current_result].reset();
    _next_buffer_idx   = 0;
    _buffer_overflowed = false;
    _results_changed   = false;
}

void Report::prepare(uint8_t vehicle_type) {
    // Get mode requirements before running any checks (in particular the mode checks require them)
    mode_util::getModeRequirements(vehicle_type, _failsafe_flags);
}

NavModes Report::getModeGroup(uint8_t nav_state) const {
    // Note: this needs to match with the json metadata definition "navigation_mode_groups"
    return (NavModes)(1u << nav_state);
}

bool Report::finalize() {
    _results[_current_result].arming_checks.valid = true;
    _already_reported                             = false;
    _results_changed                              = _results[0] != _results[1];
    return _results_changed;
}

bool Report::report(bool is_armed, bool force) {
    const hrt_abstime now            = hrt_absolute_time();
    const bool        has_difference = _had_unreported_difference || _results_changed;

    if ((now < _last_report + _min_reporting_interval) && !force) {
        if (has_difference) {
            _had_unreported_difference = true;
        }

        PX4_DEBUG("not reporting yet (%i)", has_difference);
        return false;
    }

    const bool need_to_report = (has_difference && !_already_reported) || force;

    if (!need_to_report) {
        PX4_DEBUG("not reporting, no update (%i)", has_difference);
        return false;
    }

    const Results &current_results = _results[_current_result];

    // If we have too many events, the result is still correct, we just don't report everything
    const int max_num_events = event_s::ORB_QUEUE_LENGTH - 2;

    if (_buffer_overflowed || current_results.num_events > max_num_events) {
        PX4_WARN("Too many arming check events (%i, %i > %i). Not reporting all", _buffer_overflowed,
                 current_results.num_events, max_num_events);
    }

    if (!current_results.arming_checks.valid) {
        PX4_ERR("BUG: arming checks not valid");
    }

    _last_report               = now;
    _already_reported          = true;
    _had_unreported_difference = false;

#ifdef CONSOLE_PRINT_ARMING_CHECK_EVENT
    PX4_INFO_RAW("Sending Arming/Health Report (num events: %i, can_arm: 0x%x, can_run: 0x%x)\n",
                 _results[_current_result].num_events + 2,
                 (int)current_results.arming_checks.can_arm, (int)current_results.arming_checks.can_run);
#endif

    // send arming summary

    /* EVENT
     * @arg1 chunk_idx
     * @arg2 error
     * @arg3 warning
     * @arg4 can_arm
     * @arg5 can_run
     * @type summary
     * @group arming_check
     */
    events::send<uint8_t, events::px4::enums::health_component_t, events::px4::enums::health_component_t,
                 events::px4::enums::navigation_mode_group_t, events::px4::enums::navigation_mode_group_t>(events::ID("commander_arming_check_summary"), events::Log::Protocol,
                                                                                                           "Arming check summary event", 0, current_results.arming_checks.error, current_results.arming_checks.warning,
                                                                                                           (navigation_mode_group_t)current_results.arming_checks.can_arm,
                                                                                                           (navigation_mode_group_t)current_results.arming_checks.can_run);

    // send all events
    int               offset = 0;
    events::EventType event;

    for (int i = 0; i < max_num_events && offset < _next_buffer_idx; ++i) {
        EventBufferHeader *header = (EventBufferHeader *)(_event_buffer + offset);
        memcpy(&event.id, &header->id, sizeof(event.id));
        event.log_levels = header->log_levels;
        memcpy(event.arguments, _event_buffer + offset + sizeof(EventBufferHeader), header->size);
        memset(event.arguments + header->size, 0, sizeof(event.arguments) - header->size);
        events::send(event);
        offset += sizeof(EventBufferHeader) + header->size;
#ifdef CONSOLE_PRINT_ARMING_CHECK_EVENT
        const char *message;
        memcpy(&message, &header->message, sizeof(header->message));
        PX4_INFO_RAW("   Event 0x%08" PRIx32 ": %s\n", event.id, message);
#endif
    }

    // send health summary
    /* EVENT
     * @arg1 chunk_idx
     * @arg2 is_present
     * @arg3 error
     * @arg4 warning
     * @type summary
     * @group health
     */
    events::send<uint8_t, events::px4::enums::health_component_t, events::px4::enums::health_component_t,
                 events::px4::enums::health_component_t>(events::ID("commander_health_summary"),
                                                         events::Log::Protocol,
                                                         "Health report summary event", 0, current_results.health.is_present,
                                                         current_results.health.error, current_results.health.warning);
    return true;
}
