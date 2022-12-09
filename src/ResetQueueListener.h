#pragma once

#ifndef ZEMNI_RESET_QUEUE_LISTENER_H
#define ZEMNI_RESET_QUEUE_LISTENER_H

#include "Queue.h"

namespace Zemni
{
    struct ResetQueueListener
    {
        explicit ResetQueueListener(shared_ptr <Queue> &queue) : queue(queue) {}

        void operator()(shared_ptr <Queue> new_queue)
        {
            queue.swap(new_queue);
        }

    private:
        shared_ptr <Queue> &queue;
    };
}

#endif
