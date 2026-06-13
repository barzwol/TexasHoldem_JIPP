#include <SFML/Graphics.hpp> 
#include <iostream>
#include <string>
#include "Card.h"



std::string getCardFilename(const Card& card) {
    std::string rankStr;
    int v = card.getValue();

    if (v >= 2 && v <= 10) rankStr = std::to_string(v);
    else if (v == 11) rankStr = "jack";
    else if (v == 12) rankStr = "queen";
    else if (v == 13) rankStr = "king";
    else if (v == 14) rankStr = "ace";

    std::string suitStr;
    std::string s = card.getSuit();

    if (s == "Wino" || s == "Pik") suitStr = "spades";
    else if (s == "Czerwo" || s == "Kier") suitStr = "hearts";
    else if (s == "Dzwonek" || s == "Karo") suitStr = "diamonds";
    else if (s == "Zoledz" || s == "Trefl") suitStr = "clubs";
    else suitStr = "spades";

    return "cards/" + rankStr + "_of_" + suitStr + ".png";
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Texas Hold'em - Zielony Stol");
    Card testCard(12, "Dzwonek");


    std::string filename = getCardFilename(testCard);
    std::cout << "Probuje zaladowac plik: " << filename << std::endl;

    sf::Texture cardTexture;
    if (!cardTexture.loadFromFile("P:/proj/HoldemGame/x64/Debug/cards/queen_of_diamonds.png")){
        std::cout << "Blad: Nie znalazlem pliku " << filename << "!" << std::endl;
    }

    sf::Sprite cardSprite;
    cardSprite.setTexture(cardTexture);
    cardSprite.setPosition(350.0f, 200.0f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color(35, 107, 43));

        window.draw(cardSprite);

        window.display();
    }
    return 0;
}