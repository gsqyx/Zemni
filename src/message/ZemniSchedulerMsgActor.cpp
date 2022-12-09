#include "../Common.h"
#include "ZemniSchedulerMsgActor.h"

namespace Zemni
{

    ZemniSchedulerMsgActor::ZemniSchedulerMsgActor(caf::actor_config &cfg, ZemniScheduler &scheduler) :
            event_based_actor(cfg),
            scheduler(scheduler) {}

    caf::behavior ZemniSchedulerMsgActor::make_behavior()
    {
        return {
                [=](int msgType, const string &msgContent) -> string
                {
                    return scheduler.handleMessage(msgType, msgContent);
                }
        };
    }
}
