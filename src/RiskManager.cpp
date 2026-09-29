
#include "../include/RiskManager.h"

#include <stdexcept>

// Constructor
RiskManager::RiskManager(int maxQty)
    : maxOrderQuantity(maxQty) {

    if (maxQty <= 0) {

        throw std::invalid_argument(
            "Maximum order quantity must be positive"
        );
    }
}

// Validate an order
bool RiskManager::validate(
    const Order& order,
    std::string& reason
) const {

    // Check order ID
    if (order.id <= 0) {

        reason = "Invalid order ID";

        return false;
    }

    // Check stock symbol
    if (order.symbol != "DEMO") {

        reason = "Unsupported stock symbol";

        return false;
    }

    // Check quantity
    if (order.quantity <= 0) {

        reason = "Quantity must be positive";

        return false;
    }

    // Check maximum allowed quantity
    if (order.quantity > maxOrderQuantity) {

        reason = "Order exceeds maximum allowed quantity";

        return false;
    }

    reason = "Order passed risk checks";

    return true;
}