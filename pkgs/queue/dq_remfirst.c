/*****************************************************************
 *     _   __             __   ____   _  __        __
 *    / | / /___   _  __ / /_ / __ \ (_)/ /____   / /_
 *   /  |/ // _ \ | |/_// __// /_/ // // // __ \ / __/
 *  / /|  //  __/_>  < / /_ / ____// // // /_/ // /_
 * /_/ |_/ \___//_/|_| \__//_/    /_//_/ \____/ \__/
 *
 * Copyright All Reserved © 2015-2024 NextPilot Development Team
 ******************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <queue.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: dq_remfirst
 *
 * Description:
 *   dq_remfirst removes 'node' from the head of 'queue'
 *
 ****************************************************************************/

dq_entry_t *dq_remfirst(dq_queue_t *queue) {
    dq_entry_t *ret = queue->head;

    if (ret) {
        dq_entry_t *next = ret->flink;
        if (!next) {
            queue->head = NULL;
            queue->tail = NULL;
        } else {
            queue->head = next;
            next->blink = NULL;
        }

        ret->flink = NULL;
        ret->blink = NULL;
    }

    return ret;
}