
#include "../include/OrderProcessor.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <chrono>
#include <mutex>

OrderProcessor::OrderProcessor(
    SmartRouter& r,
    RiskManager& rm,
    int workerCount
)
    : router(r),
      riskManager(rm),
      stopped(false),
      completedOrders(0) {

    if (workerCount <= 0) {
        throw std::invalid_argument(
            "Worker count must be positive"
        );
    }

    for (int i = 0; i < workerCount; ++i) {
        workers.emplace_back(
            &OrderProcessor::workerFunction,
            this
        );
    }
}

OrderProcessor::~OrderProcessor() {
    shutdown();
}

void OrderProcessor::submitOrder(const Order& order) {
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (stopped) {
            throw std::runtime_error(
                "Cannot submit after shutdown"
            );
        }

        orderQueue.push(order);
    }

    condition.notify_one();
}

void OrderProcessor::workerFunction() {
    while (true) {
        Order order(0, "", Side::BUY, 0);

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            condition.wait(lock, [this]() {
                return stopped || !orderQueue.empty();
            });

            if (stopped && orderQueue.empty()) {
                return;
            }

            order = orderQueue.front();
            orderQueue.pop();
        }

        processOrder(order);
    }
}

void OrderProcessor::processOrder(const Order& order) {
    std::string reason;

    if (!riskManager.validate(order, reason)) {
        std::lock_guard<std::mutex> lock(outputMutex);

        std::cout << "\nOrder " << order.id
                  << " REJECTED: " << reason << "\n";

        completedOrders.fetch_add(1);
        return;
    }

    std::vector<Execution> executions =
        router.route(order);

    int totalFilled = 0;
    double totalValue = 0.0;
    double totalFees = 0.0;

    for (const auto& execution : executions) {
        totalFilled += execution.quantity;

        totalValue += execution.quantity *
                      execution.price;

        totalFees += execution.fee;
    }

    int remaining = order.quantity - totalFilled;

    std::lock_guard<std::mutex> lock(outputMutex);

    std::cout << "\n================================\n";
    std::cout << "Order ID: " << order.id << "\n";

    std::cout << "Side: "
              << (order.side == Side::BUY ? "BUY" : "SELL")
              << "\n";

    std::cout << "Requested: "
              << order.quantity << "\n";

    std::cout << "Executions:\n";

    for (const auto& execution : executions) {
        std::cout << "  "
                  << execution.exchangeName
                  << " | Quantity: " << execution.quantity
                  << " | Price: " << std::fixed
                  << std::setprecision(2)
                  << execution.price
                  << " | Fee: " << execution.fee
                  << "\n";
    }

    std::cout << "Filled: " << totalFilled << "\n";
    std::cout << "Unfilled: " << remaining << "\n";

    if (order.side == Side::BUY) {
        std::cout << "Total Cost incl. fees: "
                  << totalValue + totalFees << "\n";
    } else {
        std::cout << "Net Proceeds after fees: "
                  << totalValue - totalFees << "\n";
    }

    if (remaining == 0) {
        std::cout << "Status: FILLED\n";
    } else if (totalFilled > 0) {
        std::cout << "Status: PARTIALLY FILLED\n";
    } else {
        std::cout << "Status: UNFILLED\n";
    }

    std::cout << "================================\n";

    completedOrders.fetch_add(1);
}

void OrderProcessor::shutdown() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (stopped) {
            return;
        }

        stopped = true;
    }

    condition.notify_all();

    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    workers.clear();
}

int OrderProcessor::getCompletedOrders() const {
    return completedOrders.load();
}