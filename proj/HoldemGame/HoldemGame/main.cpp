#include <iostream>
#include "Deck.h"

using namespace std;

int main() {
    try {
        cout << "--- TEXAS HOLDEM ---" << endl;

        Deck myDeck;
        cout << "Karty w talii przed dobieraniem: " << myDeck.cardsLeft() << endl;

        cout << "Tasowanie kart..." << endl;
        myDeck.shuffle();

        Card firstCard = myDeck.drawCard();

        cout << "\nTwoja wylosowana karta to: ";
        firstCard.printCard();

        cout << "Karty w talii po dobraniu: " << myDeck.cardsLeft() << endl;

    }
    catch (const std::exception& e) {
        cerr << "Wystapil blad: " << e.what() << endl;
    }

    cin.get();

    return 0;
}