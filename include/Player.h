#pragma once
#include "API.h"
#include "Wallet.h"
#include "Card.h"
#include <string>
#include <vector>
#include <memory>

class EXPORT_API Player {
private:
    std::string name;
    std::unique_ptr<Wallet> wallet;
    std::vector<Card> hand;
    bool isFolded;

public:
    Player(std::string playerName, int startingMoney);
    virtual ~Player();

    std::string getName() const;
    int getBalance() const;
    void bet(int amount);

    void receiveCard(const Card& card);
    virtual void printHand() const;

    void saveToFile() const;
    void loadFromFile();

    void win(int amount);
    void resetHand();
    void fold();
    bool hasFolded() const;

    std::vector<Card> getHand() const;
    virtual int makeDecision(int currentBet);
};