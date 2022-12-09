#include "../src/ZemniScheduler.h"
#include "../src/ZemniNodeManager.h"
#include <iostream>

using namespace Zemni;
using std::shared_ptr;

struct Producer
{
    explicit Producer(ZemniNodeManager *nodeManager) :
            rql(queue),
            nodeManager(nodeManager) {}

    void run()
    {
        queue = nodeManager->getQueueForProducer(1, rql);
        sleep(3);

        int index = 1;
        StreamElement ele;
        while (index < 2000)
        {
            ele.setValue(std::to_string(index++));
            queue->blockingWrite(ele);
            sleep(1);
        }
    }

    void start()
    {
        std::thread(&Producer::run, this).detach();
    }

private:
    shared_ptr<Queue> queue;
    ResetQueueListener rql;
    ZemniNodeManager *nodeManager;
};

struct Consumer
{
    explicit Consumer(ZemniNodeManager *nodeManager) :
            nodeManager(nodeManager) {}

    void run()
    {
        queue = nodeManager->getQueueForConsumer(1);
        int index = 0;
        StreamElement element;
        while (index++ < 500)
        {
            queue->blockingRead(element);
            cout << "Consumer " << element.value1 << "," << element.value2 << endl;
        }
    }

    void start()
    {
        std::thread(&Consumer::run, this).detach();
    }

private:
    shared_ptr<Queue> queue;
    ZemniNodeManager *nodeManager;
};

void multiNodeProducer()
{
    ZemniScheduler scheduler(20000, 30000, 40000);
    scheduler.init();
    scheduler.start();

    sleep(1);

    ZemniNodeManager nodeManager0("localhost", "localhost", 20000);
    nodeManager0.init();
    nodeManager0.start();

    ZemniNodeManager nodeManager1("localhost", "localhost", 20000);
    nodeManager1.init();
    nodeManager1.start();

    shared_ptr<Queue> queue_consumer;
    queue_consumer = nodeManager0.getQueueForConsumer(1);
    DLOG(INFO) << "Get consumer on node manager 1";
    std::thread consumer([](shared_ptr<Queue> *queue)
                         {
                             StreamElement element;
                             while (true)
                             {
                                 (*queue)->blockingRead(element);
                                 cout << "Consumer " << element.value1 << "," << element.value2 << endl;
                             }
                         }, &queue_consumer);

    shared_ptr<Queue> queue_producer0;
    ResetQueueListener rql_producer0(queue_producer0);
    queue_producer0 = nodeManager1.getQueueForProducer(1, rql_producer0);

    int index = 1;
    int count = 0;

    StreamElement ele;
    while (count++ < 5)
    {
        ele.setValue(std::to_string(index++));
        queue_producer0->blockingWrite(ele);
        sleep(1);
    }

    shared_ptr<Queue> queue_producer1;
    ResetQueueListener rql_producer1(queue_producer1);
    queue_producer1 = nodeManager0.getQueueForProducer(1, rql_producer1);

    count = 0;
    while (count++ < 5)
    {
        ele.setValue(std::to_string(index++));
        queue_producer1->blockingWrite(ele);
        sleep(1);
    }

    shared_ptr<Queue> queue_producer2;
    ResetQueueListener rql_producer2(queue_producer2);
    queue_producer2 = nodeManager1.getQueueForProducer(1, rql_producer2);
    count = 0;
    while (count++ < 5)
    {
        ele.setValue(std::to_string(index++));
        queue_producer2->blockingWrite(ele);
        sleep(1);
    }

    shared_ptr<Queue> queue_producer3;
    ResetQueueListener rql_producer3(queue_producer3);
    queue_producer3 = nodeManager0.getQueueForProducer(1, rql_producer3);
    count = 0;
    while (count++ < 5)
    {
        ele.setValue(std::to_string(index++));
        queue_producer3->blockingWrite(ele);
        sleep(1);
    }

    DLOG(INFO) << "complete";

    consumer.join();
}

void multiNodeConsumer()
{
    ZemniScheduler scheduler(20000, 30000, 40000);
    scheduler.init();
    scheduler.start();

    sleep(1);

    ZemniNodeManager nodeManager0("localhost", "localhost", 20000);
    nodeManager0.init();
    nodeManager0.start();

    ZemniNodeManager nodeManager1("localhost", "localhost", 20000);
    nodeManager1.init();
    nodeManager1.start();

    shared_ptr<Queue> queue_producer;
    ResetQueueListener rql_producer(queue_producer);
    queue_producer = nodeManager0.getQueueForProducer(1, rql_producer);

    std::thread producer([](shared_ptr<Queue> *queue)
                         {
                             sleep(3);
                             int index = 1;
                             StreamElement ele;
                             while (index < 2000)
                             {
                                 ele.setValue(std::to_string(index++));
                                 (*queue)->blockingWrite(ele);
                                 sleep(1);
                             }
                         }, &queue_producer);


    shared_ptr<Queue> queue_consumer_1;
    queue_consumer_1 = nodeManager1.getQueueForConsumer(1);

    DLOG(INFO) << "Get consumer on node manager 1";
    int index = 0;
    StreamElement element;
    while (index++ < 5)
    {
        queue_consumer_1->blockingRead(element);
        cout << "Consumer " << element.value1 << "," << element.value2 << endl;
    }

    shared_ptr<Queue> queue_consumer_0;
    queue_consumer_0 = nodeManager0.getQueueForConsumer(1);

    DLOG(INFO) << "Get consumer on node manager 0";
    index = 0;
    while (index++ < 5)
    {
        queue_consumer_0->blockingRead(element);
        cout << "Consumer " << element.value1 << "," << element.value2 << endl;
    }

    shared_ptr<Queue> queue_consumer_2;
    queue_consumer_2 = nodeManager1.getQueueForConsumer(1);

    DLOG(INFO) << "Get consumer on node manager 1";
    index = 0;
    while (index++ < 5)
    {
        queue_consumer_2->blockingRead(element);
        cout << "Consumer " << element.value1 << "," << element.value2 << endl;
    }

    shared_ptr<Queue> queue_consumer_3;
    queue_consumer_3 = nodeManager0.getQueueForConsumer(1);

    DLOG(INFO) << "Get consumer on node manager 0";
    index = 0;
    while (index++ < 5)
    {
        queue_consumer_3->blockingRead(element);
        cout << "Consumer " << element.value1 << "," << element.value2 << endl;
    }


    producer.join();
}

void multiNodeTest()
{
    ZemniScheduler scheduler(20000, 30000, 40000);
    scheduler.init();
    scheduler.start();

    sleep(1);

    ZemniNodeManager nodeManager0("localhost", "localhost", 20000);
    nodeManager0.init();
    nodeManager0.start();

    ZemniNodeManager nodeManager1("localhost", "localhost", 20000);
    nodeManager1.init();
    nodeManager1.start();

    Consumer consumer(&nodeManager0);
    consumer.start();

    shared_ptr<Queue> queue0;
    ResetQueueListener rql0(queue0);

    queue0 = nodeManager1.getQueueForProducer(1, rql0);
    DLOG(INFO) << "Get queue " << queue0->getId() << " as producer";
    int index = 0;
    StreamElement element;
    while (index++ < 5)
    {
        element.setValue(std::to_string(index++));
        queue0->blockingWrite(element);
        sleep(1);
    }

    shared_ptr<Queue> queue1;
    ResetQueueListener rql1(queue1);
    queue1 = nodeManager0.getQueueForProducer(1, rql1);
    DLOG(INFO) << "Get queue " << queue1->getId() << " as producer";
    index = 0;
    while (index++ < 500)
    {
        element.setValue(std::to_string(index++));
        queue1->blockingWrite(element);
        sleep(1);
    }
    sleep(1000);
}

int main()
{
//    singleNodeTest();

//    multiNodeTest();
    multiNodeProducer();
    multiNodeConsumer();

    return 0;
}
