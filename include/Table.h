#pragma once
#include "API.h"
#include "Card.h"
#include <vector>
#include <iostream>

class EXPORT_API Table {
private:
    std::vector<Card> communityCards;
    int pot;

public:
    Table();
    ~Table();

    void addCommunityCard(Card card);
    void addToPot(int amount);
    int getPot() const;
    void clearTable();
    void printTable() const;
};
