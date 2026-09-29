
#include "../include/SmartRouter.h"

#include <algorithm>

SmartRouter::SmartRouter(
    const std::vector<std::shared_ptr<IExchange>>& exchangeList
) : exchanges(exchangeList) {
}

std::vector<Execution> SmartRouter::route(
    const Order& order
) {
    std::lock_guard<std::mutex> lock(routingMutex);

    std::vector<Execution> executions;

    // BUY: lowest ask first.
    // SELL: highest bid first.
    if (order.side == Side::BUY) {
        std::sort(
            exchanges.begin(),
            exchanges.end(),
            [](const std::shared_ptr<IExchange>& a,
               const std::shared_ptr<IExchange>& b) {
                return a->getAskPrice() < b->getAskPrice();
            }
        );
    } else {
        std::sort(
            exchanges.begin(),
            exchanges.end(),
            [](const std::shared_ptr<IExchange>& a,
               const std::shared_ptr<IExchange>& b) {
                return a->getBidPrice() > b->getBidPrice();
            }
        );
    }

    int remaining = order.quantity;

    for (const auto& exchange : exchanges) {
        if (remaining == 0) {
            break;
        }

        double price;

        if (order.side == Side::BUY) {
            price = exchange->getAskPrice();
        } else {
            price = exchange->getBidPrice();
        }

        int filled = exchange->execute(
            order.side,
            remaining
        );

        if (filled > 0) {
            double fee =
                filled * exchange->getFeePerShare();

            executions.emplace_back(
                exchange->getName(),
                filled,
                price,
                fee
            );

            remaining -= filled;
        }
    }

    return executions;
}