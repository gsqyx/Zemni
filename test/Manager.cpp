#include "../src/ZemniScheduler.h"
#include "../src/ZemniNodeManager.h"


int main()
{
    getchar();

    Zemni::Scheduler scheduler(30000, 31000, 32000);
    scheduler.init();
    scheduler.start();

    sleep(6);

    Zemni::NodeManager nodeManager0("localhost", "localhost", 30000);
    nodeManager0.init();
    nodeManager0.start();

    sleep(3);

    std::thread([&]
                {
                    auto queue = nodeManager0.getQueueForConsumer(1);
                    StreamElement element;
                    int index = 1;
                    while (index++ < 500)
                    {
                        (*queue)->blockingRead(element);
                        cout << element.value1 << "," << element.value2 << endl;
                    }
                }).detach();

    return 0;
}
