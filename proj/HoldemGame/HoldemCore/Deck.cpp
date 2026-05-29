#include "Deck.h"
#include <algorithm>
#include <random>
#include <chrono>
#include <stdexcept>

Deck::Deck() {
    std::string suits[] = { "Wino", "Czerwo", "Zoladz", "Dzwonek" };

    for (const auto& suit : suits) {
        for (int val = 2; val <= 14; ++val) {
            cards.push_back(Card(val, suit));
        }
    }
}

Deck::~Deck() {}

void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

Card Deck::drawCard() {
    if (cards.empty()) {
        throw std::out_of_range("Blad: Talia jest pusta");
    }

    Card drawn = cards.back();
    cards.pop_back();
    return drawn;
}

int Deck::cardsLeft() const {
    return cards.size();
}