#include "HandEvaluator.h"

HandEvaluator::HandEvaluator() {}
HandEvaluator::~HandEvaluator() {}

HandRank HandEvaluator::evaluateHand(const std::vector<Card>& cards) const {
    if (cards.empty()) return HandRank::HighCard;

    std::map<int, int> rankCounts;

    std::map<std::string, int> suitCounts;

    for (const auto& card : cards) {
        rankCounts[card.getValue()]++;
        suitCounts[card.getSuit()]++;
    }

    bool hasFlush = false;
    for (const auto& suit : suitCounts) {
        if (suit.second >= 5) hasFlush = true;
    }

    int pairs = 0;
    int threes = 0;
    int fours = 0;

    for (const auto& rank : rankCounts) {
        if (rank.second == 2) pairs++;
        else if (rank.second == 3) threes++;
        else if (rank.second == 4) fours++;
    }

    if (hasFlush) return HandRank::Flush;
    if (fours > 0) return HandRank::FourOfAKind;
    if (threes > 0 && pairs > 0) return HandRank::FullHouse;
    if (threes > 0) return HandRank::ThreeOfAKind;
    if (pairs >= 2) return HandRank::TwoPair;
    if (pairs == 1) return HandRank::Pair;

    return HandRank::HighCard;
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
    case HandRank::RoyalFlush: return "Poker Krolewski";
    default: return "Nieznany uklad";
    }
}