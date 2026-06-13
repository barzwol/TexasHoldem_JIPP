#include "Bot.h"
#include <iostream>

Bot::Bot(std::string botName, int startingMoney)
    : Player(botName, startingMoney) {
}

Bot::~Bot() {}

void Bot::printHand() const {
    std::cout << "--- Karty przeciwnika: " << getName() << " (Kasa: " << getBalance() << "$) ---" << std::endl;
    std::cout << "1. [Ukryta karta]" << std::endl;
    std::cout << "2. [Ukryta karta]\n" << std::endl;
}