#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Card.h"
#include "Deck.h"
#include "HandEvaluator.h"
#include "Player.h" 
#include "Bot.h"    

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

    return "P:/proj/HoldemGame/x64/Debug/cards/" + rankStr + "_of_" + suitStr + ".png";
}

enum class GamePhase { PreFlop, Flop, Turn, River, Showdown };

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Texas Hold'em - Zielony Stol");

    int pot = 0;
    bool payoutDone = false;

    std::string customRaiseStr = "150";

    bool showAllInConfirm = false;
    int pendingAllInAmount = 0;

    Player human("gracz", 1000);
    human.loadFromFile();
    int playerCash = human.getBalance();

    Bot bot("bot", 1000);
    bot.loadFromFile();
    int botCash = bot.getBalance();

    auto saveGame = [&]() {
        Player p("gracz", playerCash); p.saveToFile();
        Bot b("bot", botCash); b.saveToFile();
        };

    sf::SoundBuffer shuffleBuffer; shuffleBuffer.loadFromFile("P:/proj/HoldemGame/x64/Debug/shuffle.wav");
    sf::Sound shuffleSound; shuffleSound.setBuffer(shuffleBuffer);

    sf::SoundBuffer chipsBuffer; chipsBuffer.loadFromFile("P:/proj/HoldemGame/x64/Debug/chips.wav");
    sf::Sound chipsSound; chipsSound.setBuffer(chipsBuffer);

    sf::SoundBuffer foldBuffer; foldBuffer.loadFromFile("P:/proj/HoldemGame/x64/Debug/fold.wav");
    sf::Sound foldSound; foldSound.setBuffer(foldBuffer);

    sf::Texture bgTexture; bgTexture.loadFromFile("P:/proj/HoldemGame/x64/Debug/carpet.jpg");
    sf::Sprite bgSprite(bgTexture);
    if (bgTexture.getSize().x > 0) bgSprite.setScale(1280.0f / bgTexture.getSize().x, 720.0f / bgTexture.getSize().y);

    sf::Texture backTexture; backTexture.loadFromFile("P:/proj/HoldemGame/x64/Debug/cards/card_back.png");
    sf::Font font; font.loadFromFile("P:/proj/HoldemGame/x64/Debug/rockwell.ttf");

    float spacing = 120.0f; float scale = 0.2f; float backScale = 0.2f;

    Deck deck; deck.shuffle();
    std::vector<Card> botCards, communityCards, playerCards;

    botCards.push_back(deck.drawCard()); botCards.push_back(deck.drawCard());
    for (int i = 0; i < 5; ++i) communityCards.push_back(deck.drawCard());
    playerCards.push_back(deck.drawCard()); playerCards.push_back(deck.drawCard());

    GamePhase currentPhase = GamePhase::PreFlop;

    sf::Texture botTextures[2], commTextures[5], playerTextures[2];
    sf::Sprite botSprites[2], commSprites[5], playerSprites[2];

    auto loadCardGraphics = [&]() {
        for (int i = 0; i < 2; ++i) {
            botTextures[i].loadFromFile(getCardFilename(botCards[i]));
            botSprites[i].setTexture(backTexture); botSprites[i].setScale(backScale, backScale); botSprites[i].setPosition(520.0f + (i * spacing), 10.0f);

            playerTextures[i].loadFromFile(getCardFilename(playerCards[i]));
            playerSprites[i].setTexture(playerTextures[i]); playerSprites[i].setScale(scale, scale); playerSprites[i].setPosition(520.0f + (i * spacing), 565.0f);
        }
        for (int i = 0; i < 5; ++i) {
            commTextures[i].loadFromFile(getCardFilename(communityCards[i]));
            commSprites[i].setTexture(backTexture); commSprites[i].setScale(backScale, backScale); commSprites[i].setPosition(340.0f + (i * spacing), 280.0f);
        }
        };
    loadCardGraphics();

    sf::Text botText("", font, 22); botText.setPosition(750.0f, 70.0f);
    sf::Text potText("", font, 28); potText.setPosition(520.0f, 220.0f); potText.setFillColor(sf::Color::Yellow);
    sf::Text playerText("", font, 22); playerText.setPosition(300.0f, 620.0f);
    sf::Text resultText("", font, 36); resultText.setPosition(350.0f, 430.0f);

    sf::Text botActionText("BOT: CZEKA", font, 24);
    botActionText.setPosition(520.0f, 160.0f);
    botActionText.setFillColor(sf::Color(600, 200, 200));

    sf::Text typingInfo("Pisz na klawiaturze, aby zmienic stawke:", font, 16);
    typingInfo.setPosition(980.0f, 650.0f);
    typingInfo.setFillColor(sf::Color(500, 200, 200));

    sf::RectangleShape resetBtn(sf::Vector2f(120.0f, 40.0f)); resetBtn.setPosition(20.0f, 20.0f); resetBtn.setFillColor(sf::Color(100, 100, 100));
    sf::Text resetText("RESET $", font, 20); resetText.setPosition(35.0f, 28.0f);

    sf::RectangleShape foldBtn(sf::Vector2f(160.0f, 50.0f)); foldBtn.setPosition(1050.0f, 450.0f); foldBtn.setFillColor(sf::Color(180, 40, 40));
    sf::Text foldText("FOLD", font, 24); foldText.setPosition(1095.0f, 460.0f);

    sf::RectangleShape callBtn(sf::Vector2f(160.0f, 50.0f)); sf::Text callText("CALL (50$)", font, 22);

    sf::RectangleShape raiseBtn(sf::Vector2f(160.0f, 50.0f)); raiseBtn.setPosition(1050.0f, 590.0f); raiseBtn.setFillColor(sf::Color(40, 140, 40));
    sf::Text raiseText("", font, 22);


    sf::RectangleShape confirmBox(sf::Vector2f(560.0f, 200.0f));
    confirmBox.setPosition(360.0f, 260.0f);
    confirmBox.setFillColor(sf::Color(30, 30, 30, 240));
    confirmBox.setOutlineThickness(3.0f);
    confirmBox.setOutlineColor(sf::Color(200, 150, 0));

    sf::Text confirmText("Czy chcesz zagrac ALL-IN?", font, 26);
    confirmText.setPosition(420.0f, 290.0f);
    confirmText.setFillColor(sf::Color::White);

    sf::RectangleShape yesBtn(sf::Vector2f(140.0f, 50.0f)); yesBtn.setPosition(450.0f, 370.0f); yesBtn.setFillColor(sf::Color(40, 140, 40));
    sf::Text yesText("TAK", font, 24); yesText.setPosition(495.0f, 380.0f);

    sf::RectangleShape noBtn(sf::Vector2f(140.0f, 50.0f)); noBtn.setPosition(690.0f, 370.0f); noBtn.setFillColor(sf::Color(180, 40, 40));
    sf::Text noText("NIE", font, 24); noText.setPosition(735.0f, 380.0f);

    shuffleSound.play();

    auto executeRaise = [&](int amount) {
        chipsSound.play();
        playerCash -= amount; pot += amount;

        Bot aiBrain("bot", botCash);
        aiBrain.receiveCard(botCards[0]);
        aiBrain.receiveCard(botCards[1]);

        int botDecision = aiBrain.makeDecision(amount);

        if (botDecision == -1) {
            foldSound.play();
            botActionText.setString("BOT: FOLD");
            currentPhase = GamePhase::Showdown;
            resultText.setString("WYGRYWASZ PULE.");
            resultText.setFillColor(sf::Color::Green);
            playerCash += pot; pot = 0; payoutDone = true;
            saveGame();
        }
        else {
            int botActualBet = std::min(amount, botCash);
            botCash -= botActualBet; pot += botActualBet;
            botActionText.setString("BOT: CALL/ALL-IN");

            currentPhase = GamePhase::Showdown;
        }
        };
    while (window.isOpen()) {

        potText.setString("Aktualna Pula: " + std::to_string(pot) + "$");
        playerText.setString("gracz (Kasa: " + std::to_string(playerCash) + "$)");
        botText.setString("bot (Kasa: " + std::to_string(botCash) + "$)");

        std::string displayRaise = customRaiseStr.empty() ? "0" : customRaiseStr;
        raiseText.setString("RAISE (" + displayRaise + "$)");
        raiseText.setPosition(1055.0f, 600.0f);

        if (currentPhase == GamePhase::Showdown) {
            callBtn.setSize(sf::Vector2f(230.0f, 50.0f)); callBtn.setPosition(1010.0f, 520.0f);
            callText.setString("NASTEPNA RUNDA"); callText.setCharacterSize(18); callText.setPosition(1030.0f, 533.0f);
            callBtn.setFillColor(sf::Color(200, 150, 0));
        }
        else {
            callBtn.setSize(sf::Vector2f(160.0f, 50.0f)); callBtn.setPosition(1050.0f, 520.0f);
            callText.setString("CALL (50$)"); callText.setCharacterSize(22); callText.setPosition(1065.0f, 530.0f);
            callBtn.setFillColor(sf::Color(60, 60, 180));
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                saveGame();
                window.close();
            }

            if (event.type == sf::Event::TextEntered && currentPhase != GamePhase::Showdown && !showAllInConfirm) {
                if (event.text.unicode == '\b') {
                    if (!customRaiseStr.empty()) customRaiseStr.pop_back();
                }
                else if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    if (customRaiseStr.length() < 5) {
                        customRaiseStr += static_cast<char>(event.text.unicode);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

                if (showAllInConfirm) {
                    if (yesBtn.getGlobalBounds().contains(mousePos)) {
                        showAllInConfirm = false;
                        executeRaise(pendingAllInAmount);
                    }
                    else if (noBtn.getGlobalBounds().contains(mousePos)) {
                        showAllInConfirm = false;
                    }
                    continue;
                }

                if (resetBtn.getGlobalBounds().contains(mousePos)) {
                    shuffleSound.play();
                    playerCash = 1000; botCash = 1000; pot = 0; payoutDone = true;
                    currentPhase = GamePhase::Showdown;
                    resultText.setString("KONTA ZRESETOWANE! KLIKNIJ NASTEPNA RUNDA.");
                    resultText.setFillColor(sf::Color::White);
                    botActionText.setString("BOT: RESTART");
                    saveGame();
                }
                else if (foldBtn.getGlobalBounds().contains(mousePos) && currentPhase != GamePhase::Showdown) {
                    foldSound.play();
                    currentPhase = GamePhase::Showdown;
                    resultText.setString("SPASOWALES! PRZECIWNIK ZGARNIA PULE.");
                    resultText.setFillColor(sf::Color::Red);
                    botCash += pot; pot = 0; payoutDone = true;
                    saveGame();
                }
                else if (callBtn.getGlobalBounds().contains(mousePos)) {
                    if (currentPhase == GamePhase::Showdown) {
                        shuffleSound.play();
                        pot = 0; payoutDone = false; currentPhase = GamePhase::PreFlop;
                        resultText.setString(""); botActionText.setString("BOT: CZEKA");

                        deck = Deck(); deck.shuffle();
                        botCards.clear(); communityCards.clear(); playerCards.clear();

                        botCards.push_back(deck.drawCard()); botCards.push_back(deck.drawCard());
                        for (int i = 0; i < 5; ++i) communityCards.push_back(deck.drawCard());
                        playerCards.push_back(deck.drawCard()); playerCards.push_back(deck.drawCard());

                        loadCardGraphics();
                    }
                    else {

                        if (playerCash >= 50 && botCash >= 50) {
                            chipsSound.play();
                            playerCash -= 50; pot += 50;
                            int currentBet = 50;

                            Bot aiBrain("bot", botCash);
                            aiBrain.receiveCard(botCards[0]); aiBrain.receiveCard(botCards[1]);
                            int botDecision = aiBrain.makeDecision(currentBet);

                            if (botDecision == -1) {
                                foldSound.play();
                                botActionText.setString("BOT: FOLD");
                                currentPhase = GamePhase::Showdown;
                                resultText.setString("WYGRYWASZ PULE.");
                                resultText.setFillColor(sf::Color::Green);
                                playerCash += pot; pot = 0; payoutDone = true;
                                saveGame();
                            }
                            else if (botDecision == currentBet) {
                                botActionText.setString("BOT: CALL");
                                botCash -= currentBet; pot += currentBet;

                                if (currentPhase == GamePhase::PreFlop) currentPhase = GamePhase::Flop;
                                else if (currentPhase == GamePhase::Flop) currentPhase = GamePhase::Turn;
                                else if (currentPhase == GamePhase::Turn) currentPhase = GamePhase::River;
                                else if (currentPhase == GamePhase::River) currentPhase = GamePhase::Showdown;
                            }
                            else if (botDecision >= currentBet) {
                                botActionText.setString("BOT: CALL");
                                int botActualBet = std::min(currentBet, botCash);
                                botCash -= botActualBet; pot += botActualBet;

                                if (botCash <= 0 || playerCash <= 0) {
                                    currentPhase = GamePhase::Showdown;
                                }
                                else {
                                    if (currentPhase == GamePhase::PreFlop) currentPhase = GamePhase::Flop;
                                    else if (currentPhase == GamePhase::Flop) currentPhase = GamePhase::Turn;
                                    else if (currentPhase == GamePhase::Turn) currentPhase = GamePhase::River;
                                    else if (currentPhase == GamePhase::River) currentPhase = GamePhase::Showdown;
                                }
                            }
                        }
                    }
                }
                else if (raiseBtn.getGlobalBounds().contains(mousePos) && currentPhase != GamePhase::Showdown) {
                    int customBet = customRaiseStr.empty() ? 0 : std::stoi(customRaiseStr);

                    if (customBet > 0) {
                        int maxPossibleBet = std::min(playerCash, botCash);

                        if (customBet >= maxPossibleBet) {
                            pendingAllInAmount = maxPossibleBet;
                            if (pendingAllInAmount > 0) {
                                confirmText.setString("Chcesz zagrac ALL-IN za " + std::to_string(pendingAllInAmount) + "$?");
                                showAllInConfirm = true;
                            }
                        }
                        else {
                            executeRaise(customBet);
                        }
                    }
                }
            }
        }

        if (currentPhase == GamePhase::Showdown && !payoutDone && resultText.getString() == "") {
            HandEvaluator evaluator;
            std::vector<Card> pHand = playerCards; pHand.insert(pHand.end(), communityCards.begin(), communityCards.end());
            std::vector<Card> bHand = botCards; bHand.insert(bHand.end(), communityCards.begin(), communityCards.end());

            HandResult pResult = evaluator.evaluateHand(pHand);
            HandResult bResult = evaluator.evaluateHand(bHand);

            if (pResult > bResult) {
                resultText.setString("WYGRYWASZ (" + evaluator.rankToString(pResult.rank) + ")! ZGARNIASZ PULE!");
                resultText.setFillColor(sf::Color::Green);
                playerCash += pot;
            }
            else if (bResult > pResult) {
                resultText.setString("WYGRYWA PRZECIWNIK (" + evaluator.rankToString(bResult.rank) + ")!");
                resultText.setFillColor(sf::Color::Red);
                botCash += pot;
            }
            else {
                resultText.setString("REMIS! PODZIAL PULI!");
                resultText.setFillColor(sf::Color::Yellow);
                playerCash += pot / 2;
                botCash += pot / 2;
            }
            pot = 0;
            payoutDone = true;

            saveGame();
        }

        for (int i = 0; i < 5; ++i) {
            bool reveal = false;
            if (currentPhase == GamePhase::Flop && i < 3) reveal = true;
            if (currentPhase == GamePhase::Turn && i < 4) reveal = true;
            if ((currentPhase == GamePhase::River || currentPhase == GamePhase::Showdown) && i < 5) reveal = true;

            if (reveal) {
                commSprites[i].setTexture(commTextures[i]); commSprites[i].setScale(scale, scale);
            }
            else {
                commSprites[i].setTexture(backTexture); commSprites[i].setScale(backScale, backScale);
            }
        }

        for (int i = 0; i < 2; ++i) {
            if (currentPhase == GamePhase::Showdown) {
                botSprites[i].setTexture(botTextures[i]); botSprites[i].setScale(scale, scale);
            }
            else {
                botSprites[i].setTexture(backTexture); botSprites[i].setScale(backScale, backScale);
            }
        }

        window.clear();
        window.draw(bgSprite);
        for (int i = 0; i < 2; ++i) window.draw(botSprites[i]);
        for (int i = 0; i < 5; ++i) window.draw(commSprites[i]);
        for (int i = 0; i < 2; ++i) window.draw(playerSprites[i]);
        window.draw(potText); window.draw(botText); window.draw(playerText); window.draw(botActionText);

        window.draw(resetBtn); window.draw(resetText);
        window.draw(typingInfo);
        window.draw(foldBtn); window.draw(foldText);
        window.draw(callBtn); window.draw(callText);
        window.draw(raiseBtn); window.draw(raiseText);

        if (currentPhase == GamePhase::Showdown) window.draw(resultText);

        if (showAllInConfirm) {
            window.draw(confirmBox);
            window.draw(confirmText);
            window.draw(yesBtn); window.draw(yesText);
            window.draw(noBtn); window.draw(noText);
        }

        window.display();
    }

    return 0;
}