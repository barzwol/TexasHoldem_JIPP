#pragma once
#include "API.h"
#include <string>


class EXPORT_API Card {
private:
    int value;
    std::string suit;

public:
    Card(int val, std::string st);
    ~Card();

    int getValue() const;
    std::string getSuit() const;
    void printCard() const;
};
