
#ifndef RISK_MANAGER_H
#define RISK_MANAGER_H

#include "Order.h"
#include <string>

class RiskManager {
private:
    int maxOrderQuantity;

public:
    explicit RiskManager(int maxQty);

    bool validate(const Order& order,
                  std::string& reason) const;
};

#endif