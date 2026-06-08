#include "Player.h"
#include <iostream>
#include <stdexcept>

Player::Player(std::string playerName, int startingMoney)
    : name(playerName), isFolded(false) {
    wallet = std::make_unique<Wallet>(startingMoney);
}

Player::~Player() {}

std::string Player::getName() const {
    return name;
}

int Player::getBalance() const {
    return wallet->getBalance();
}

bool Player::hasFolded() const {
    return isFolded;
}

void Player::receiveCard(const Card& card) {
    if (hand.size() >= 2) {
        throw std::logic_error("Gracz moze miec maksymalnie dwie karty w rece!");
    }
    hand.push_back(card);
}

void Player::fold() {
    isFolded = true;
}

void Player::resetHand() {
    hand.clear();
    isFolded = false;
}

void Player::bet(int amount) {
    wallet->bet(amount);
}

void Player::win(int amount) {
    wallet->addFunds(amount);
}

void Player::printHand() const {
    std::cout << name << " - karty: ";
    if (hand.empty()) {
        std::cout << "Pusta";
    }
    else {
        for (const auto& card : hand) {
            std::cout << "[" << card.getValue() << " " << card.getSuit() << "] ";
        }
    }
    std::cout << " | Stan konta: " << getBalance() << "$" << std::endl;
}