#include "Bot.h"
#include <iostream>
#include <cstdlib> 

int Bot::makeDecision(int currentBet) {
    if (hasFolded()) return -1;

    std::cout << "\n[ " << getName() << " analizuje swoje karty... ]" << std::endl;

    auto myCards = getHand();
    bool strongHand = false;
    bool mediumHand = false;

    if (myCards.size() == 2) {
        Card c1 = myCards[0];
        Card c2 = myCards[1];

        if (c1.getValue() == c2.getValue()) {
            strongHand = true;
        }
        else if (c1.getValue() >= 11 && c2.getValue() >= 11) {
            strongHand = true;
        }
        else if (c1.getValue() >= 12 || c2.getValue() >= 12 || c1.getSuit() == c2.getSuit()) {
            mediumHand = true;
        }
    }

    int action = rand() % 100;
    int finalDecision = 0;

    if (strongHand) {
        if (action < 60) {
            int raiseAmount = 100;
            std::cout << getName() << " podbija (Raise) o " << raiseAmount << "$!" << std::endl;
            finalDecision = currentBet + raiseAmount;
        }
        else {
            std::cout << getName() << " sprawdza (Call) za " << currentBet << "$." << std::endl;
            finalDecision = currentBet;
        }
    }
    else if (mediumHand) {
        if (action < 10 && currentBet > 50) {
            std::cout << getName() << " pasuje (Fold)." << std::endl;
            fold();
            return -1;
        }
        else if (action < 35) {
            int raiseAmount = 50;
            std::cout << getName() << " podbija (Raise) o " << raiseAmount << "$." << std::endl;
            finalDecision = currentBet + raiseAmount;
        }
        else {
            std::cout << getName() << " sprawdza (Call) za " << currentBet << "$." << std::endl;
            finalDecision = currentBet;
        }
    }
    else {
        if (action < 45 && currentBet > 0) {
            std::cout << getName() << " pasuje (Fold)." << std::endl;
            fold();
            return -1;
        }
        else if (action < 90) {
            std::cout << getName() << " sprawdza (Call) za " << currentBet << "$." << std::endl;
            finalDecision = currentBet;
        }
        else {
            std::cout << getName() << " podbija o 50$!" << std::endl;
            finalDecision = currentBet + 50;
        }
    }

    if (finalDecision >= getBalance()) {
        std::cout << getName() << " nie ma kasy na pelne zagranie. Wchodzi ALL-IN za " << getBalance() << "$!" << std::endl;
        return getBalance();
    }

    return finalDecision;
}
Bot::Bot(std::string botName, int startingMoney)
    : Player(botName, startingMoney) {
}

Bot::~Bot() {}

void Bot::printHand() const {
    std::cout << "--- Karty przeciwnika: " << getName() << " (Kasa: " << getBalance() << "$) ---" << std::endl;
    std::cout << "1. [Ukryta karta]" << std::endl;
    std::cout << "2. [Ukryta karta]\n" << std::endl;
}