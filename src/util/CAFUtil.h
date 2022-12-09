#pragma once

#include "../Common.h"
#include "../Config.h"
#include <caf/io/middleman.hpp>
#include <caf/scoped_actor.hpp>
#include <caf/actor_system_config.hpp>
#include <glog/logging.h>

namespace Zemni
{
    class CAFUtil
    {
    private:
        shared_ptr<caf::actor_system> actor_system;
        caf::actor_system_config actor_system_config;

    private:
        CAFUtil()
        {
            actor_system_config.load<caf::io::middleman>();
            actor_system = make_shared<caf::actor_system>(actor_system_config);
        }

    public:
        CAFUtil(CAFUtil const &) = delete;

        void operator=(CAFUtil const &) = delete;

        static CAFUtil &getInstance()
        {
            static CAFUtil instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
            return instance;
        }

    public:
        shared_ptr<caf::actor_system> getActorSystem()
        {
            return actor_system;
        }

        caf::actor spawnRemoteActor(string host, uint16_t port)
        {
            auto remote_actor = actor_system->middleman().remote_actor(std::move(host), port);
            CHECK(remote_actor) << "Spawn remote actor fail! port=" << port;
            return remote_actor.value();
        }

        string sendMessage(string host, uint16_t port, int msgType, string &&msgContent)
        {
            auto remote_actor = actor_system->middleman().remote_actor(std::move(host), port);
            CHECK(remote_actor) << "Spawn actor fail!";
            return sendMessage(remote_actor.value(), msgType, std::forward<string>(msgContent));
        }

        string sendMessage(caf::actor &dstActor, int msgType, string &&msgContent)
        {
            string msg;
            caf::scoped_actor scopedActor{*actor_system};
            scopedActor->request(dstActor,
                                 std::chrono::seconds(ZEMNI_NETWORK_SECONDS_TIMEOUT),
                                 msgType,
                                 msgContent).receive
                    (
                            [&](const string &what)
                            {
                                msg = what;
                            },
                            [&](caf::error err)
                            {
                                cerr << to_string(err) << endl;
                            }
                    );
            return msg;
        }

        void sendMessageAsync(string host, uint16_t port, int msgType, string &&msgContent)
        {
            auto remote_actor = actor_system->middleman().remote_actor(std::move(host), port);
            CHECK(remote_actor) << "Spawn remote actor fail! port=" << port;
            sendMessageAsync(remote_actor.value(), msgType, std::forward<string>(msgContent));
        }

        void sendMessageAsync(caf::actor &dstActor, int msgType, string &&msgContent)
        {
            std::thread([&](caf::actor dstActor, int msgType, string msgContent)
                        {
                            sendMessage(dstActor, msgType, std::forward<string>(msgContent));
                        }, dstActor, msgType, msgContent).detach();
        }
    };
}
