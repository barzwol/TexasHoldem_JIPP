#include "Card.h"
#include <iostream>

Card::Card(int val, std::string st) : value(val), suit(st) {}

Card::~Card() {}

int Card::getValue() const {
    return value;
}

std::string Card::getSuit() const {
    return suit;
}

void Card::printCard() const {
    std::cout << "Karta: " << value << " " << suit << std::endl;
}