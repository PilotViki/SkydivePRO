#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string>

// Enum pentru tipuri de echipament (C++98 compatible)
enum EquipmentType {
    RIG,
    ALTIMETER,
    HELMET,
    JUMPSUIT
};

class Equipment {
private:
    int equipmentID;
    EquipmentType type;
    std::string size;  // Relevant for jumpsuits, helmets, rigs
    double pricePerDay;
    bool available;
    int loyaltyPointsAwarded;
    static int nextEquipmentID;

public:
    // Constructors
    Equipment();
    Equipment(EquipmentType type, const std::string& size, double pricePerDay, int loyaltyPointsAwarded);
    
    // Getters
    int getEquipmentID() const;
    EquipmentType getType() const;
    std::string getTypeAsString() const;
    std::string getSize() const;
    double getPricePerDay() const;
    bool isAvailable() const;
    int getLoyaltyPointsAwarded() const;
    
    // Setters
    void setType(EquipmentType type);
    void setSize(const std::string& size);
    void setPricePerDay(double price);
    void setAvailability(bool available);
    void setLoyaltyPointsAwarded(int points);
    
    // Calculate rental cost for a given number of days
    double calculateRentalCost(int days) const;
};

#endif // EQUIPMENT_H
