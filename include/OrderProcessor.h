
#ifndef ORDER_PROCESSOR_H
#define ORDER_PROCESSOR_H

#include "Order.h"
#include "SmartRouter.h"
#include "RiskManager.h"

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class OrderProcessor {
private:
    SmartRouter& router;
    RiskManager& riskManager;

    std::queue<Order> orderQueue;

    std::mutex queueMutex;
    std::mutex outputMutex;

    std::condition_variable condition;

    std::vector<std::thread> workers;

    bool stopped;

    std::atomic<int> completedOrders;

    void workerFunction();
    void processOrder(const Order& order);

public:
    OrderProcessor(SmartRouter& r,
                   RiskManager& rm,
                   int workerCount);

    ~OrderProcessor();

    void submitOrder(const Order& order);

    void shutdown();

    int getCompletedOrders() const;
};

#endif