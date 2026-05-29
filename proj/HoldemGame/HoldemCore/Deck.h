#pragma once
#include "API.h"
#include "Card.h"
#include <vector>

class EXPORT_API Deck {
private:
    std::vector<Card> cards;

public:
    Deck();
    ~Deck();

    void shuffle();
    Card drawCard();
    int cardsLeft() const;
};
