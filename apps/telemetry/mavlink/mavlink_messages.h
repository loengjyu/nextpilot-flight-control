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
 * @file mavlink_messages.h
 * MAVLink 1.0 message formatters definition.
 *
 * @author Anton Babushkin <anton.babushkin@me.com>
 */

#ifndef MAVLINK_MESSAGES_H_
#define MAVLINK_MESSAGES_H_

#include "mavlink_stream.h"

#define DEFINE_GET_PX4_CUSTOM_MODE
#include <commander/px4_custom_mode.h>

class StreamListItem {
public:
    MavlinkStream *(*new_instance)(Mavlink *mavlink);
    const char *name;
    uint16_t    id;

    StreamListItem(MavlinkStream *(*inst)(Mavlink *mavlink), const char *_name, uint16_t _id) :
        new_instance(inst),
        name(_name),
        id(_id) {
    }

    const char *get_name() const {
        return name;
    }
    uint16_t get_id() const {
        return id;
    }
};

template <class T>
static StreamListItem create_stream_list_item() {
    return StreamListItem(&T::new_instance, T::get_name_static(), T::get_id_static());
}

const char *get_stream_name(const uint16_t msg_id);

MavlinkStream *create_mavlink_stream(const char *stream_name, Mavlink *mavlink);

MavlinkStream *create_mavlink_stream(const uint16_t msg_id, Mavlink *mavlink);

union px4_custom_mode get_px4_custom_mode(uint8_t nav_state);

#endif /* MAVLINK_MESSAGES_H_ */