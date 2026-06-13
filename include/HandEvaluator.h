#pragma once
#include "API.h"
#include "Card.h"
#include <vector>
#include <string>
#include <map>

enum class HandRank {
    HighCard, Pair, TwoPair, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush, RoyalFlush
};

struct EXPORT_API HandResult {
    HandRank rank;
    std::vector<int> tieBreakers;

    bool operator>(const HandResult& other) const {
        if (rank != other.rank) return rank > other.rank;
        return tieBreakers > other.tieBreakers;
    }
    bool operator==(const HandResult& other) const {
        return rank == other.rank && tieBreakers == other.tieBreakers;
    }
};

class EXPORT_API HandEvaluator {
public:
    HandEvaluator();
    ~HandEvaluator();

    HandResult evaluateHand(const std::vector<Card>& cards) const;
    std::string rankToString(HandRank rank) const;
};