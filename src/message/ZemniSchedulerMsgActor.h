#pragma once

#include <caf/event_based_actor.hpp>
#include "../ZemniScheduler.h"


namespace Zemni
{
    class ZemniScheduler;

    class ZemniSchedulerMsgActor : public caf::event_based_actor
    {
    private:
        ZemniScheduler & scheduler;

    public:
        ZemniSchedulerMsgActor(caf::actor_config &cfg, ZemniScheduler& scheduler);

    protected:
        caf::behavior make_behavior() override;
    };

}
