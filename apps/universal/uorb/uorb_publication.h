/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

#ifndef __UORB_PUBLICATION_H__
#define __UORB_PUBLICATION_H__

#include "uorb_device_node.h"

#ifdef __cplusplus

namespace nextpilot::uORB {
template <typename U>
class DefaultQueueSize {
private:
    template <typename T>
    static constexpr uint8_t get_queue_size(decltype(T::ORB_QUEUE_LENGTH) *) {
        return T::ORB_QUEUE_LENGTH;
    }

    template <typename T>
    static constexpr uint8_t get_queue_size(...) {
        return 1;
    }

public:
    static constexpr unsigned value = get_queue_size<U>(nullptr);
};

class PublicationBase {
public:
    bool advertised() const {
        return _handle != nullptr;
    }

    bool unadvertise() {
        // return (Manager::orb_unadvertise(_handle) == PX4_OK);
        // _handle->
    }

    orb_id_t get_topic() const {
        return get_orb_meta(_orb_id);
    }

protected:
    PublicationBase(ORB_ID id) :
        _orb_id(id) {
    }

    ~PublicationBase() {
        if (_handle != nullptr) {
            // don't automatically unadvertise queued publications (eg vehicle_command)
            if (Manager::orb_get_queue_size(_handle) == 1) {
                unadvertise();
            }
        }
    }

    orb_advert_t _handle{nullptr};
    const ORB_ID _orb_id;
};

} // namespace nextpilot::uORB

#endif //__cplusplus

#endif // __UORB_PUBLICATION_H__