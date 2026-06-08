#include <iostream>
#include "Deck.h"
#include "Player.h"

int main() {
    try {
        std::cout << "--- POKER START ---" << std::endl;

        Deck tableDeck;
        tableDeck.shuffle();

        Player player1("gracz", 1000);

        player1.receiveCard(tableDeck.drawCard());
        player1.receiveCard(tableDeck.drawCard());

        player1.printHand();

        std::cout << "\ngracz stawia 200$" << std::endl;
        player1.bet(200);

        std::cout << "Nowy stan konta: " << player1.getBalance() << "$" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Wystapil blad: " << e.what() << std::endl;
    }

    std::cin.get();
    return 0;
}