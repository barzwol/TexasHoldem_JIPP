#include "Wallet.h"

Wallet::Wallet(int initialBalance) : balance(initialBalance) {}

Wallet::~Wallet() {}

int Wallet::getBalance() const {
    return balance;
}

void Wallet::addFunds(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Blad: Nie mozna dodac ujemnej kwoty do portfela");
    }
    balance += amount;
}

void Wallet::bet(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Blad: Zaklad nie moze byc ujemny");
    }
    if (amount > balance) {
        throw std::runtime_error("Blad: Niewystarczajace srodki na koncie");
    }
    balance -= amount;
}