
#ifndef EXCHANGE_H
#define EXCHANGE_H

#include <string>
#include "Order.h"

class IExchange {
public:
    virtual ~IExchange() = default;

    virtual std::string getName() const = 0;
    virtual double getBidPrice() const = 0;
    virtual double getAskPrice() const = 0;

    virtual int getBidQuantity() const = 0;
    virtual int getAskQuantity() const = 0;

    virtual double getFeePerShare() const = 0;

    virtual int execute(Side side, int quantity) = 0;
};

class Exchange : public IExchange {
private:
    std::string name;

    double bidPrice;
    double askPrice;

    int bidQuantity;
    int askQuantity;

    double feePerShare;

public:
    Exchange(const std::string& exchangeName,
             double bid, double ask,
             int bidQty, int askQty,
             double fee);

    std::string getName() const override;
    double getBidPrice() const override;
    double getAskPrice() const override;

    int getBidQuantity() const override;
    int getAskQuantity() const override;

    double getFeePerShare() const override;

    int execute(Side side, int quantity) override;
};

#endif