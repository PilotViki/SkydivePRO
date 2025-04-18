#include "../include/Administrator.h"
#include "../include/SkydiveSystem.h"

Administrator::Administrator() : User(), accessLevel("none") {
}

Administrator::Administrator(const std::string& name, const std::string& email, const std::string& password, 
                           const std::string& accessLevel)
    : User(name, email, password), accessLevel(accessLevel) {
}

Administrator::~Administrator() {
    // Specific admin cleanup if needed
}

std::string Administrator::getRole() const {
    return "Administrator";
}

std::string Administrator::getAccessLevel() const {
    return accessLevel;
}

void Administrator::setAccessLevel(const std::string& level) {
    accessLevel = level;
}

bool Administrator::confirmReservation(int reservationID) {
    // Check if admin has proper access level
    if (accessLevel != "full" && accessLevel != "reservation-only") {
        return false;
    }
    
    // Get the system instance
    auto system = SkydiveSystem::getInstance();
    
    // Update the reservation status
    return system->updateReservationStatus(reservationID, ReservationStatus::CONFIRMED);
}

bool Administrator::cancelReservation(int reservationID) {
    // Check if admin has proper access level
    if (accessLevel != "full" && accessLevel != "reservation-only") {
        return false;
    }
    
    // Get the system instance
    auto system = SkydiveSystem::getInstance();
    
    // Cancel the reservation
    return system->cancelReservation(reservationID);
}

bool Administrator::addEquipment(const std::string& type, const std::string& size, double price) {
    // Check if admin has proper access level
    if (accessLevel != "full" && accessLevel != "equipment-only") {
        return false;
    }
    
    // Get the system instance
    auto system = SkydiveSystem::getInstance();
    
    // Convert string type to enum
    EquipmentType equipType;
    if (type == "RIG") {
        equipType = EquipmentType::RIG;
    } else if (type == "ALTIMETER") {
        equipType = EquipmentType::ALTIMETER;
    } else if (type == "HELMET") {
        equipType = EquipmentType::HELMET;
    } else if (type == "JUMPSUIT") {
        equipType = EquipmentType::JUMPSUIT;
    } else {
        return false; // Invalid type
    }
    
    // Add the equipment (default loyalty points)
    return system->addEquipment(equipType, size, price, 1);
}

bool Administrator::updateEquipmentStatus(int equipmentID, const std::string& status) {
    // Check if admin has proper access level
    if (accessLevel != "full" && accessLevel != "equipment-only") {
        return false;
    }
    
    // Get the system instance
    auto system = SkydiveSystem::getInstance();
    
    // Get the equipment
    auto equipment = system->getEquipment(equipmentID);
    if (!equipment) {
        return false;
    }
    
    // Update status
    equipment->setAvailability(status == "available");
    
    return true;
}
