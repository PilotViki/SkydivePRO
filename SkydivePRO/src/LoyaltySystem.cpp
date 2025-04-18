#include "../include/LoyaltySystem.h"

// Implementări goale pentru funcțiile de prevenire a copierii
LoyaltySystem::LoyaltySystem(const LoyaltySystem&) {}
LoyaltySystem& LoyaltySystem::operator=(const LoyaltySystem&) { return *this; }

// Initialize the static instance to nullptr
LoyaltySystem* LoyaltySystem::instance = nullptr;

LoyaltySystem::LoyaltySystem()
    : pointsToMoneyRatio(10.0),  // 10 points = 1 currency unit discount
      moneyToPointsRatio(50.0) { // 50 currency units = 1 point
}

LoyaltySystem* LoyaltySystem::getInstance() {
    if (instance == nullptr) {
        instance = new LoyaltySystem();
    }
    return instance;
}

int LoyaltySystem::getClientPoints(int clientID) const {
    auto it = clientPointsMap.find(clientID);
    if (it != clientPointsMap.end()) {
        return it->second;
    }
    return 0;
}

void LoyaltySystem::addPoints(int clientID, int points) {
    if (points <= 0) {
        return;
    }
    
    // Add to existing points or create new entry
    clientPointsMap[clientID] += points;
}

bool LoyaltySystem::usePoints(int clientID, int points) {
    if (points <= 0) {
        return false;
    }
    
    auto it = clientPointsMap.find(clientID);
    if (it != clientPointsMap.end() && it->second >= points) {
        it->second -= points;
        return true;
    }
    
    return false;
}

double LoyaltySystem::calculateDiscount(int points) const {
    if (points <= 0 || pointsToMoneyRatio <= 0) {
        return 0.0;
    }
    
    return points / pointsToMoneyRatio;
}

int LoyaltySystem::calculatePointsFromPurchase(double amount) const {
    if (amount <= 0 || moneyToPointsRatio <= 0) {
        return 0;
    }
    
    return static_cast<int>(amount / moneyToPointsRatio);
}

void LoyaltySystem::setPointsToMoneyRatio(double ratio) {
    if (ratio > 0) {
        pointsToMoneyRatio = ratio;
    }
}

void LoyaltySystem::setMoneyToPointsRatio(double ratio) {
    if (ratio > 0) {
        moneyToPointsRatio = ratio;
    }
}

double LoyaltySystem::getPointsToMoneyRatio() const {
    return pointsToMoneyRatio;
}

double LoyaltySystem::getMoneyToPointsRatio() const {
    return moneyToPointsRatio;
}
