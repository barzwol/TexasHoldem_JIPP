#pragma once
#include "API.h"
#include "Card.h"
#include "Wallet.h"
#include <string>
#include <vector>
#include <memory>

class EXPORT_API Player {
private:
    std::string name;
    std::vector<Card> hand;
    std::unique_ptr<Wallet> wallet;
    bool isFolded;

public:
    Player(std::string playerName, int startingMoney);
    ~Player();

    std::string getName() const;
    int getBalance() const;
    bool hasFolded() const;

    void receiveCard(const Card& card);
    void fold();
    void resetHand();
    void bet(int amount);
    void win(int amount);

    void printHand() const;
};
