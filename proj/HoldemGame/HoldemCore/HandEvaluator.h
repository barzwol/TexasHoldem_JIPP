#pragma once
#include "API.h"
#include "Card.h"
#include <vector>
#include <string>
#include <map>

enum class HandRank {
    HighCard,
    Pair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
    RoyalFlush
};

class EXPORT_API HandEvaluator {
public:
    HandEvaluator();
    ~HandEvaluator();

    HandRank evaluateHand(const std::vector<Card>& cards) const;

    std::string rankToString(HandRank rank) const;
};