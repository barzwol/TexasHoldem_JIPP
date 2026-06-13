#include "Game.h"
#include "HandEvaluator.h"
#include <iostream>

Game::Game() {}
Game::~Game() {}

void Game::removeBankruptPlayers() {
    auto it = players.begin();
    while (it != players.end()) {
        if ((*it)->getBalance() <= 0) {
            std::cout << "\n[!] Gracz " << (*it)->getName() << " ZBANKRUTOWAL i opuszcza stol! [!]" << std::endl;
            it = players.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Game::addPlayer(std::unique_ptr<Player> player) {
    player->loadFromFile();
    players.push_back(std::move(player));
}

void Game::bettingRound(const std::string& roundName) {
    std::cout << "\n--- LICYTACJA: " << roundName << " ---" << std::endl;
    int currentBet = 0;

    int activePlayers = 0;
    for (auto& p : players) {
        if (!p->hasFolded()) activePlayers++;
    }
    if (activePlayers < 2) return;

    for (auto& player : players) {
        if (player->hasFolded()) continue;

        int decision = player->makeDecision(currentBet);

        if (decision != -1) {
            player->bet(decision);
            table.addToPot(decision);
            if (decision > currentBet) {
                currentBet = decision;
            }
        }
    }
    std::cout << "Koniec licytacji (" << roundName << "). Pula: " << table.getPot() << "$\n" << std::endl;
}

void Game::start() {
    std::cout << "=== ROZPOCZYNAMY GRE W TEXAS HOLD'EM ===" << std::endl;
    removeBankruptPlayers();

    if (players.size() < 2) {
        std::cout << "Zbyt malo graczy przy stole, zeby kontynuowac gre. Koniec gry!" << std::endl;
        return;
    }

    deck.shuffle();
    table.clearTable();

    for (auto& player : players) {
        player->bet(50);
        table.addToPot(50);
    }
    std::cout << "Pula poczatkowa (Ante): " << table.getPot() << "$\n" << std::endl;

    for (auto& player : players) {
        player->resetHand();
        player->receiveCard(deck.drawCard());
        player->receiveCard(deck.drawCard());
        player->printHand();
    }
    bettingRound("PRE-FLOP");

    std::cout << "\nKrupier wyklada FLOP (3 karty)..." << std::endl;
    table.addCommunityCard(deck.drawCard());
    table.addCommunityCard(deck.drawCard());
    table.addCommunityCard(deck.drawCard());
    table.printTable();
    bettingRound("FLOP");

    std::cout << "\nKrupier wyklada TURN (4. karta)..." << std::endl;
    table.addCommunityCard(deck.drawCard());
    table.printTable();
    bettingRound("TURN");

    std::cout << "\nKrupier wyklada RIVER (5. karta)..." << std::endl;
    table.addCommunityCard(deck.drawCard());
    table.printTable();
    bettingRound("RIVER");

    std::cout << "=== WYNIKI RUNDY (SHOWDOWN) ===" << std::endl;
    HandEvaluator evaluator;
    auto communityCards = table.getCommunityCards();

    std::vector<Player*> winners;
    HandResult bestResult;
    bestResult.rank = HandRank::HighCard;

    bool firstActivePlayer = true;

    for (const auto& player : players) {
        if (player->hasFolded()) continue;

        std::cout << "\n[" << player->getName() << " pokazuje karty]:" << std::endl;
        for (const auto& card : player->getHand()) {
            card.printCard();
        }

        std::vector<Card> allCards = player->getHand();
        allCards.insert(allCards.end(), communityCards.begin(), communityCards.end());

        HandResult playerResult = evaluator.evaluateHand(allCards);
        std::cout << "=> Uklad: " << evaluator.rankToString(playerResult.rank) << std::endl;

        if (firstActivePlayer || playerResult > bestResult) {
            bestResult = playerResult;
            winners.clear();
            winners.push_back(player.get());
            firstActivePlayer = false;
        }
        else if (playerResult == bestResult) {
            winners.push_back(player.get());
        }
    }

    if (!winners.empty()) {
        int totalPot = table.getPot();
        int share = totalPot / winners.size();

        if (winners.size() > 1) {
            std::cout << "\n[!!!] REMIS. Pula zostaje podzielona! [!!!]" << std::endl;
        }

        for (auto* winner : winners) {
            std::cout << "\n!!! WYGRYWA: " << winner->getName() << " (" << evaluator.rankToString(bestResult.rank) << ") !!!" << std::endl;
            std::cout << winner->getName() << " zgarnia " << share << "$ z podzielonej puli!" << std::endl;

            winner->win(share);
            std::cout << "Nowy stan konta: " << winner->getBalance() << "$" << std::endl;
        }
        table.clearTable();
    }
    else {
        std::cout << "\nWszyscy spasowali!" << std::endl;
    }

    std::cout << "\n--- TRWA ZAPISYWANIE STANU KONTA ---" << std::endl;
    for (const auto& player : players) {
        player->saveToFile();
    }
    std::cout << "Zapisano. Mozesz bezpiecznie zamknac gre." << std::endl;
    std::cout << "===========================" << std::endl;
}