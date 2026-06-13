#include "Player.h"
#include <iostream>
#include <stdexcept>
#include <fstream>

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
    std::cout << "--- Karty gracza: " << name << " (Kasa: " << getBalance() << "$) ---" << std::endl;
    for (const auto& card : hand) {
        card.printCard();
    }
    std::cout << std::endl;
}
std::vector<Card> Player::getHand() const {
    return hand;
}

int Player::makeDecision(int currentBet) {
    if (isFolded) return -1;
    if (getBalance() == 0) return 0;

    int choice;
    std::cout << "\n[ TWOJA KOLEJ: " << name << " | Kasa: " << getBalance() << "$ ]" << std::endl;
    std::cout << "Obecna stawka: " << currentBet << "$" << std::endl;
    std::cout << "1. Czekaj/Sprawdz (Call)\n2. Podbij (Raise)\n3. Spasuj (Fold)\nWybierz (1-3): ";
    std::cin >> choice;

    if (choice == 3) {
        fold();
        std::cout << name << " pasuje." << std::endl;
        return -1;
    }
    else if (choice == 2) {
        int raiseAmount;
        std::cout << "O ile chcesz podbic?: ";
        std::cin >> raiseAmount;
        int totalBet = currentBet + raiseAmount;

        if (totalBet > getBalance()) {
            std::cout << "Nie masz tyle kasy! Wchodzisz ALL-IN za " << getBalance() << "$!" << std::endl;
            return getBalance();
        }
        return totalBet;
    }

    if (currentBet >= getBalance()) {
        std::cout << name << " nie ma kasy na pelne sprawdzenie. Wchodzi ALL-IN za " << getBalance() << "$!" << std::endl;
        return getBalance();
    }

    std::cout << name << " sprawdza za " << currentBet << "$." << std::endl;
    return currentBet;
}

void Player::saveToFile() const {
    std::string filename = name + "_save.bin";
    std::ofstream file(filename, std::ios::binary);

    if (file.is_open()) {
        int currentMoney = getBalance();
        file.write(reinterpret_cast<const char*>(&currentMoney), sizeof(currentMoney));
        file.close();
    }
}

void Player::loadFromFile() {
    std::string filename = name + "_save.bin";
    std::ifstream file(filename, std::ios::binary);

    if (file.is_open()) {
        int savedMoney;
        if (file.read(reinterpret_cast<char*>(&savedMoney), sizeof(savedMoney))) {
            int currentMoney = getBalance();
            if (savedMoney > currentMoney) {
                win(savedMoney - currentMoney);
            }
            else if (savedMoney < currentMoney) {
                bet(currentMoney - savedMoney);
            }
        }
        file.close();
    }
}