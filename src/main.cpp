
#include "../include/Order.h"
#include "../include/Exchange.h"
#include "../include/SmartRouter.h"
#include "../include/RiskManager.h"
#include "../include/OrderProcessor.h"

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

int main() {
    try {
        // 1. Create simulated exchanges
        std::vector<std::shared_ptr<IExchange>> exchanges;

        exchanges.push_back(
            std::make_shared<Exchange>(
                "Exchange_A",
                249.50, 250.00,
                100, 100,
                0.10
            )
        );

        exchanges.push_back(
            std::make_shared<Exchange>(
                "Exchange_B",
                248.50, 249.00,
                80, 40,
                0.20
            )
        );

        exchanges.push_back(
            std::make_shared<Exchange>(
                "Exchange_C",
                249.00, 249.50,
                200, 100,
                0.05
            )
        );

        // 2. Create router and risk manager
        SmartRouter router(exchanges);

        RiskManager riskManager(1000);

        // 3. Create 3 worker threads
        OrderProcessor processor(
            router,
            riskManager,
            3
        );

        // 4. Generate demo orders
        const int totalOrders = 20;

        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 1; i <= totalOrders; ++i) {
            Side side;

            if (i % 2 == 0) {
                side = Side::SELL;
            } else {
                side = Side::BUY;
            }

            Order order(
                i,
                "DEMO",
                side,
                10
            );

            processor.submitOrder(order);
        }

        // 5. Wait for all orders to finish
        processor.shutdown();

        auto end = std::chrono::high_resolution_clock::now();

        // 6. Calculate simulator throughput
        double elapsedSeconds =
            std::chrono::duration<double>(
                end - start
            ).count();

        int completed = processor.getCompletedOrders();

        std::cout << "\n\n========== SUMMARY ==========\n";

        std::cout << "Total orders submitted: "
                  << totalOrders << "\n";

        std::cout << "Orders completed: "
                  << completed << "\n";

        std::cout << "Time taken: "
                  << elapsedSeconds << " seconds\n";

        if (elapsedSeconds > 0) {
            std::cout << "Simulator throughput: "
                      << completed / elapsedSeconds
                      << " orders/second\n";
        }

        std::cout << "=============================\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}