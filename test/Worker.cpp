#include "../src/ZemniScheduler.h"
#include "../src/ZemniNodeManager.h"

int main()
{
    getchar();

    sleep(3);

    NodeManager nodeManager0("localhost", "localhost", 30000);
    nodeManager0.init();
    nodeManager0.start();

    std::thread([&]
                {
                    auto *queue0 = nodeManager0.getQueueForProducer(1);

                    sleep(3);

                    int index = 1;
                    StreamElement ele;
                    while (index < 500)
                    {
                        ele.setValue(std::to_string(index++));
                        (*queue0)->blockingWrite(ele);
                        sleep(1);
                    }
                }).detach();

    return 0;
}
