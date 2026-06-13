#pragma once
#include "API.h"
#include "Player.h"

class EXPORT_API Bot : public Player {
public:
    Bot(std::string botName, int startingMoney);
    ~Bot() override;

    void printHand() const override;
};
