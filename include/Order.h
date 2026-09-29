
#ifndef ORDER_H
#define ORDER_H

#include <string>

enum class Side {
    BUY,
    SELL
};

struct Order {
    int id;
    std::string symbol;
    Side side;
    int quantity;

    Order(int orderId, const std::string& stock,
          Side orderSide, int qty)
        : id(orderId),
          symbol(stock),
          side(orderSide),
          quantity(qty) {
    }
};

struct Execution {
    std::string exchangeName;
    int quantity;
    double price;
    double fee;

    Execution(const std::string& name, int qty,
              double executionPrice, double executionFee)
        : exchangeName(name),
          quantity(qty),
          price(executionPrice),
          fee(executionFee) {
    }
};

#endif