#pragma once
#include "API.h"
#include <stdexcept>

class EXPORT_API Wallet {
private:
    int balance;

public:
    Wallet(int initialBalance);
    ~Wallet();

    int getBalance() const;
    void addFunds(int amount);
    void bet(int amount);
};
