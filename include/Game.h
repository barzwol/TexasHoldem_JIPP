#pragma once
#include "API.h"
#include "Deck.h"
#include "Table.h"
#include "Player.h"
#include <vector>
#include <memory>
#include <string>

class EXPORT_API Game {
private:
    Deck deck;
    Table table;
    std::vector<std::unique_ptr<Player>> players;

    void bettingRound(const std::string& roundName);
    void removeBankruptPlayers();

public:
    Game();
    ~Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void addPlayer(std::unique_ptr<Player> player);
    void start();
};