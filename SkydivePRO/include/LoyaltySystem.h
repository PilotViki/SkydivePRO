#ifndef LOYALTY_SYSTEM_H
#define LOYALTY_SYSTEM_H

#include <map>

class LoyaltySystem {
private:
    static LoyaltySystem* instance;
    std::map<int, int> clientPointsMap;  // Maps clientID to points
    double pointsToMoneyRatio; // How many points needed for 1 unit of currency discount
    double moneyToPointsRatio; // How much money spent to earn 1 point
    
    // Private constructor (Singleton pattern)
    LoyaltySystem();

public:
    // Singleton accessor
    static LoyaltySystem* getInstance();
    
    // Prevent copying (C++98 compatible)
    LoyaltySystem(const LoyaltySystem&); // nu este implementată
    LoyaltySystem& operator=(const LoyaltySystem&); // nu este implementată
    
    // Points management
    int getClientPoints(int clientID) const;
    void addPoints(int clientID, int points);
    bool usePoints(int clientID, int points);
    
    // Conversion methods
    double calculateDiscount(int points) const;
    int calculatePointsFromPurchase(double amount) const;
    
    // Configuration
    void setPointsToMoneyRatio(double ratio);
    void setMoneyToPointsRatio(double ratio);
    double getPointsToMoneyRatio() const;
    double getMoneyToPointsRatio() const;
};

#endif // LOYALTY_SYSTEM_H
