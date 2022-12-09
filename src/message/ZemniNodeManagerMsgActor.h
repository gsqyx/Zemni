#pragma once

#include "../ZemniNodeManager.h"
#include <caf/event_based_actor.hpp>

namespace Zemni
{
    class ZemniNodeManagerMsgActor : public caf::event_based_actor
    {
    private:
        ZemniNodeManager &nodeManager;
    public:
        ZemniNodeManagerMsgActor(caf::actor_config &cfg, ZemniNodeManager &nodeManager);

    protected:
        caf::behavior make_behavior() override;
    };
}
