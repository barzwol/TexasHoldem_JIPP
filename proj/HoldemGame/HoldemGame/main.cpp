#include <iostream>
#include <memory>
#include "Game.h"
#include "Player.h"
#include "Bot.h"

int main() {
    try {
        Game texasHoldem;

        texasHoldem.addPlayer(std::make_unique<Player>("gracz", 1000));
        texasHoldem.addPlayer(std::make_unique<Bot>("bot", 1000));

        texasHoldem.start();

    }
    catch (const std::exception& e) {
        std::cerr << "Wystapil krytyczny blad: " << e.what() << std::endl;
    }

    std::cout << "\nWcisnij Enter, aby zakonczyc...";
    std::cin.get();
    return 0;
}