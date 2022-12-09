#include "ZemniNodeManagerMsgActor.h"
#include "../Common.h"

namespace Zemni
{
    ZemniNodeManagerMsgActor::ZemniNodeManagerMsgActor(caf::actor_config &cfg, Zemni::ZemniNodeManager &nodeManager) :
            caf::event_based_actor(cfg),
            nodeManager(nodeManager) {}

    caf::behavior ZemniNodeManagerMsgActor::make_behavior()
    {
        return {
                [=](int msgType, const string &msgContent) -> string
                {
                    return nodeManager.handleMessage(msgType, msgContent);
                }
        };
    }
}
