
#ifndef SMART_ROUTER_H
#define SMART_ROUTER_H

#include "Exchange.h"
#include "Order.h"

#include <vector>
#include <memory>
#include <mutex>

class SmartRouter {
private:
    std::vector<std::shared_ptr<IExchange>> exchanges;

    // Protects shared exchange liquidity and prices.
    std::mutex routingMutex;

public:
    explicit SmartRouter(
        const std::vector<std::shared_ptr<IExchange>>& exchangeList
    );

    std::vector<Execution> route(const Order& order);
};

#endif