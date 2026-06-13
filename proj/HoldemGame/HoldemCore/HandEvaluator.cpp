#include "HandEvaluator.h"
#include <algorithm> 

HandEvaluator::HandEvaluator() {}
HandEvaluator::~HandEvaluator() {}

HandResult HandEvaluator::evaluateHand(const std::vector<Card>& cards) const {
    HandResult result;
    result.rank = HandRank::HighCard;
    if (cards.empty()) return result;

    std::map<int, int> rankCounts;
    std::map<std::string, int> suitCounts;

    for (const auto& card : cards) {
        rankCounts[card.getValue()]++;
        suitCounts[card.getSuit()]++;
    }

    std::string flushSuit = "";
    bool hasFlush = false;
    for (const auto& suit : suitCounts) {
        if (suit.second >= 5) {
            hasFlush = true;
            flushSuit = suit.first;
        }
    }

    int pairs = 0, threes = 0, fours = 0;
    for (const auto& rank : rankCounts) {
        if (rank.second == 2) pairs++;
        else if (rank.second == 3) threes++;
        else if (rank.second == 4) fours++;
    }

    auto findStraightHigh = [](const std::map<int, int>& rc) {
        std::vector<int> ur;
        for (auto it = rc.rbegin(); it != rc.rend(); ++it) ur.push_back(it->first);

        for (size_t i = 0; i + 4 < ur.size(); ++i) {
            if (ur[i] - ur[i + 4] == 4) return ur[i];
        }
        if (rc.count(14) && rc.count(5) && rc.count(4) && rc.count(3) && rc.count(2)) {
            return 5;
        }
        return 0;
        };

    int straightHigh = findStraightHigh(rankCounts);

    int straightFlushHigh = 0;
    if (hasFlush) {
        std::map<int, int> flushRankCounts;
        for (const auto& card : cards) {
            if (card.getSuit() == flushSuit) flushRankCounts[card.getValue()]++;
        }
        straightFlushHigh = findStraightHigh(flushRankCounts);
    }

    if (straightFlushHigh > 0) {
        if (straightFlushHigh == 14) result.rank = HandRank::RoyalFlush;
        else result.rank = HandRank::StraightFlush;
    }
    else if (fours > 0) result.rank = HandRank::FourOfAKind;
    else if (threes > 0 && pairs > 0) result.rank = HandRank::FullHouse;
    else if (hasFlush) result.rank = HandRank::Flush;
    else if (straightHigh > 0) result.rank = HandRank::Straight;
    else if (threes > 0) result.rank = HandRank::ThreeOfAKind;
    else if (pairs >= 2) result.rank = HandRank::TwoPair;
    else if (pairs == 1) result.rank = HandRank::Pair;
    else result.rank = HandRank::HighCard;

    if (result.rank == HandRank::RoyalFlush || result.rank == HandRank::StraightFlush) {
        result.tieBreakers.push_back(straightFlushHigh);
    }
    else if (result.rank == HandRank::Straight) {
        result.tieBreakers.push_back(straightHigh);
    }
    else if (result.rank == HandRank::Flush) {
        std::vector<int> flushValues;
        for (const auto& card : cards) {
            if (card.getSuit() == flushSuit) flushValues.push_back(card.getValue());
        }
        std::sort(flushValues.begin(), flushValues.end(), std::greater<int>());
        for (size_t i = 0; i < 5 && i < flushValues.size(); ++i) result.tieBreakers.push_back(flushValues[i]);
    }
    else {
        std::vector<std::pair<int, int>> sortedRanks;
        for (const auto& rank : rankCounts) sortedRanks.push_back({ rank.second, rank.first });

        std::sort(sortedRanks.begin(), sortedRanks.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
            if (a.first != b.first) return a.first > b.first;
            return a.second > b.second;
            });

        int cardsAdded = 0;
        for (const auto& p : sortedRanks) {
            for (int i = 0; i < p.first && cardsAdded < 5; ++i) {
                result.tieBreakers.push_back(p.second);
                cardsAdded++;
            }
        }
    }

    return result;
}

std::string HandEvaluator::rankToString(HandRank rank) const {
    switch (rank) {
    case HandRank::HighCard: return "Wysoka karta";
    case HandRank::Pair: return "Para";
    case HandRank::TwoPair: return "Dwie pary";
    case HandRank::ThreeOfAKind: return "Trojka";
    case HandRank::Straight: return "Strit";
    case HandRank::Flush: return "Kolor";
    case HandRank::FullHouse: return "Full House";
    case HandRank::FourOfAKind: return "Kareta";
    case HandRank::StraightFlush: return "Poker";
    case HandRank::RoyalFlush: return "Poker Krolewski!!!";
    default: return "Nieznany uklad";
    }
}