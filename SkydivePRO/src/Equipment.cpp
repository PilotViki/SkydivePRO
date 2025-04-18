#include "../include/Equipment.h"

// Initialize static member
int Equipment::nextEquipmentID = 1;

Equipment::Equipment() 
    : equipmentID(0), 
      type(EquipmentType::RIG), 
      size(""), 
      pricePerDay(0.0), 
      available(true), 
      loyaltyPointsAwarded(0) {
}

Equipment::Equipment(EquipmentType type, const std::string& size, double pricePerDay, int loyaltyPointsAwarded)
    : equipmentID(nextEquipmentID++), 
      type(type), 
      size(size), 
      pricePerDay(pricePerDay), 
      available(true), 
      loyaltyPointsAwarded(loyaltyPointsAwarded) {
}

int Equipment::getEquipmentID() const {
    return equipmentID;
}

EquipmentType Equipment::getType() const {
    return type;
}

std::string Equipment::getTypeAsString() const {
    switch (type) {
        case EquipmentType::RIG:
            return "RIG";
        case EquipmentType::ALTIMETER:
            return "ALTIMETER";
        case EquipmentType::HELMET:
            return "HELMET";
        case EquipmentType::JUMPSUIT:
            return "JUMPSUIT";
        default:
            return "UNKNOWN";
    }
}

std::string Equipment::getSize() const {
    return size;
}

double Equipment::getPricePerDay() const {
    return pricePerDay;
}

bool Equipment::isAvailable() const {
    return available;
}

int Equipment::getLoyaltyPointsAwarded() const {
    return loyaltyPointsAwarded;
}

void Equipment::setType(EquipmentType type) {
    this->type = type;
}

void Equipment::setSize(const std::string& size) {
    this->size = size;
}

void Equipment::setPricePerDay(double price) {
    if (price >= 0) {
        this->pricePerDay = price;
    }
}

void Equipment::setAvailability(bool available) {
    this->available = available;
}

void Equipment::setLoyaltyPointsAwarded(int points) {
    if (points >= 0) {
        this->loyaltyPointsAwarded = points;
    }
}

double Equipment::calculateRentalCost(int days) const {
    if (days <= 0) {
        return 0.0;
    }
    return pricePerDay * days;
}
