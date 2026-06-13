#include "Card.h"
#include <iostream>

Card::Card(int v, std::string s) : value(v), suit(s) {}

Card::~Card() {}

int Card::getValue() const {
    return value;
}

std::string Card::getSuit() const {
    return suit;
}

void Card::printCard() const {
    std::string faceName;

    switch (value) {
    case 11: faceName = "Jupek"; break;
    case 12: faceName = "Dama"; break;
    case 13: faceName = "Krol"; break;
    case 14: faceName = "As"; break;
    default: faceName = std::to_string(value); break;
    }

    std::cout << "- " << faceName << " " << suit << std::endl;
}