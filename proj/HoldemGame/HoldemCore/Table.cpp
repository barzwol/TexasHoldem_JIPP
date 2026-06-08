#include "Table.h"

Table::Table() : pot(0) {}

Table::~Table() {}

void Table::addCommunityCard(Card card) {
    if (communityCards.size() < 5) {
        communityCards.push_back(card);
    }
    else {
        std::cout << "Blad: Na stole jest juz 5 kart!" << std::endl;
    }
}

void Table::addToPot(int amount) {
    if (amount > 0) {
        pot += amount;
    }
}

int Table::getPot() const {
    return pot;
}

void Table::clearTable() {
    communityCards.clear();
    pot = 0;
}

void Table::printTable() const {
    std::cout << "\n=== STOL (Pula: " << pot << "$) ===" << std::endl;
    if (communityCards.empty()) {
        std::cout << "Brak kart na stole." << std::endl;
    }
    else {
        std::cout << "Karty wspolne:" << std::endl;
        for (const auto& card : communityCards) {
            card.printCard();
        }
    }
    std::cout << "=========================\n" << std::endl;
}