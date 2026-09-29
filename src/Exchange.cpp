
#include "../include/Exchange.h"
#include <stdexcept>
#include <Order.h>

Exchange::Exchange(const std::string& exchangeName,
                   double bid, double ask,
                   int bidQty, int askQty,
                   double fee)
    : name(exchangeName),
      bidPrice(bid),
      askPrice(ask),
      bidQuantity(bidQty),
      askQuantity(askQty),
      feePerShare(fee) {

    if (name.empty() || bid <= 0 || ask <= 0 ||
        bid > ask || bidQty < 0 || askQty < 0 ||
        fee < 0) {
        throw std::invalid_argument("Invalid exchange data");
    }
}

std::string Exchange::getName() const {
    return name;
}

double Exchange::getBidPrice() const {
    return bidPrice;
}

double Exchange::getAskPrice() const {
    return askPrice;
}

int Exchange::getBidQuantity() const {
    return bidQuantity;
}

int Exchange::getAskQuantity() const {
    return askQuantity;
}

double Exchange::getFeePerShare() const {
    return feePerShare;
}

int Exchange::execute(Side side, int quantity) {
    if (quantity <= 0) {
        return 0;
    }

    if (side == Side::BUY) {
        int filled = quantity < askQuantity
                     ? quantity : askQuantity;

        askQuantity -= filled;
        return filled;
    }

    int filled = quantity < bidQuantity
                 ? quantity : bidQuantity;

    bidQuantity -= filled;
    return filled;
}