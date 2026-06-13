#include "Game.h"
#include "HandEvaluator.h"
#include <iostream>

Game::Game() {}
Game::~Game() {}

void Game::addPlayer(std::unique_ptr<Player> player) {
    players.push_back(std::move(player));
}

void Game::start() {
    std::cout << "=== ROZPOCZYNAMY GRE W TEXAS HOLD'EM ===" << std::endl;

    deck.shuffle();
    table.clearTable();

    std::cout << "--- Pobieranie wpisowego ---" << std::endl;
    for (auto& player : players) {
        player->bet(50);
        table.addToPot(50);
    }
    std::cout << "Pula na stole wynosi: " << table.getPot() << "$\n" << std::endl;

    for (auto& player : players) {
        player->resetHand();
        player->receiveCard(deck.drawCard());
        player->receiveCard(deck.drawCard());
    }

    for (const auto& player : players) {
        player->printHand();
    }

    std::cout << "\nKrupier wyklada 5 kart wspolnych..." << std::endl;
    for (int i = 0; i < 5; i++) {
        table.addCommunityCard(deck.drawCard());
    }

    table.printTable();

    std::cout << "=== WYNIKI RUNDY ===" << std::endl;
    HandEvaluator evaluator;
    auto communityCards = table.getCommunityCards();

    Player* winner = nullptr;
    HandRank bestRank = HandRank::HighCard;

    for (const auto& player : players) {
        std::vector<Card> allCards = player->getHand();
        allCards.insert(allCards.end(), communityCards.begin(), communityCards.end());

        HandRank rank = evaluator.evaluateHand(allCards);
        std::cout << player->getName() << " ulozyl: " << evaluator.rankToString(rank) << std::endl;

        if (winner == nullptr || rank > bestRank) {
            bestRank = rank;
            winner = player.get();
        }
    }

    if (winner != nullptr) {
        int pot = table.getPot();
        std::cout << "\n!!! WYGRYWA: " << winner->getName() << " (" << evaluator.rankToString(bestRank) << ") !!!" << std::endl;
        std::cout << winner->getName() << " zgarnia " << pot << "$ z puli!" << std::endl;

        winner->win(pot);
        table.clearTable();

        std::cout << "Nowy stan konta zwyciezcy: " << winner->getBalance() << "$" << std::endl;
    }

    std::cout << "===========================" << std::endl;
}