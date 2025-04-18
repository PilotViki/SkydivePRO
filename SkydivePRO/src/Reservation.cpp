#include "../include/Reservation.h"
#include <algorithm>

// Initialize static member
int Reservation::nextReservationID = 1;

Reservation::Reservation()
    : reservationID(0),
      clientID(0),
      planeSlotID(-1),
      creationDate(std::time(nullptr)),
      reservationDate(0),
      status(ReservationStatus::PENDING),
      totalCost(0.0),
      loyaltyPointsEarned(0) {
}

Reservation::Reservation(int clientID, const std::vector<int>& equipmentIDs, int planeSlotID, std::time_t reservationDate)
    : reservationID(nextReservationID++),
      clientID(clientID),
      equipmentIDs(equipmentIDs),
      planeSlotID(planeSlotID),
      creationDate(std::time(nullptr)),
      reservationDate(reservationDate),
      status(ReservationStatus::PENDING),
      totalCost(0.0),
      loyaltyPointsEarned(0) {
}

int Reservation::getReservationID() const {
    return reservationID;
}

int Reservation::getClientID() const {
    return clientID;
}

const std::vector<int>& Reservation::getEquipmentIDs() const {
    return equipmentIDs;
}

int Reservation::getPlaneSlotID() const {
    return planeSlotID;
}

std::time_t Reservation::getCreationDate() const {
    return creationDate;
}

std::time_t Reservation::getReservationDate() const {
    return reservationDate;
}

ReservationStatus Reservation::getStatus() const {
    return status;
}

std::string Reservation::getStatusAsString() const {
    switch (status) {
        case ReservationStatus::PENDING:
            return "PENDING";
        case ReservationStatus::CONFIRMED:
            return "CONFIRMED";
        case ReservationStatus::CANCELLED:
            return "CANCELLED";
        case ReservationStatus::IN_PROGRESS:
            return "IN_PROGRESS";
        case ReservationStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "UNKNOWN";
    }
}

double Reservation::getTotalCost() const {
    return totalCost;
}

int Reservation::getLoyaltyPointsEarned() const {
    return loyaltyPointsEarned;
}

void Reservation::setClientID(int clientID) {
    this->clientID = clientID;
}

void Reservation::addEquipment(int equipmentID) {
    // Check if equipment ID is already in the list
    if (std::find(equipmentIDs.begin(), equipmentIDs.end(), equipmentID) == equipmentIDs.end()) {
        equipmentIDs.push_back(equipmentID);
    }
}

void Reservation::removeEquipment(int equipmentID) {
    equipmentIDs.erase(
        std::remove(equipmentIDs.begin(), equipmentIDs.end(), equipmentID),
        equipmentIDs.end()
    );
}

void Reservation::setPlaneSlotID(int slotID) {
    planeSlotID = slotID;
}

void Reservation::setReservationDate(std::time_t date) {
    reservationDate = date;
}

void Reservation::setStatus(ReservationStatus status) {
    this->status = status;
}

void Reservation::setTotalCost(double cost) {
    if (cost >= 0) {
        totalCost = cost;
    }
}

void Reservation::setLoyaltyPointsEarned(int points) {
    if (points >= 0) {
        loyaltyPointsEarned = points;
    }
}

bool Reservation::updateStatus(ReservationStatus newStatus) {
    // Implement status transition rules
    bool validTransition = false;
    
    switch (status) {
        case ReservationStatus::PENDING:
            // From pending, we can only confirm or cancel
            validTransition = (newStatus == ReservationStatus::CONFIRMED || 
                              newStatus == ReservationStatus::CANCELLED);
            break;
            
        case ReservationStatus::CONFIRMED:
            // From confirmed, we can cancel, start, or complete
            validTransition = (newStatus == ReservationStatus::CANCELLED || 
                              newStatus == ReservationStatus::IN_PROGRESS ||
                              newStatus == ReservationStatus::COMPLETED);
            break;
            
        case ReservationStatus::IN_PROGRESS:
            // From in progress, we can only complete
            validTransition = (newStatus == ReservationStatus::COMPLETED);
            break;
            
        case ReservationStatus::COMPLETED:
        case ReservationStatus::CANCELLED:
            // Cannot change status once completed or cancelled
            validTransition = false;
            break;
    }
    
    if (validTransition) {
        status = newStatus;
    }
    
    return validTransition;
}

bool Reservation::cancelReservation() {
    // Can only cancel pending or confirmed reservations
    if (status == ReservationStatus::PENDING || status == ReservationStatus::CONFIRMED) {
        status = ReservationStatus::CANCELLED;
        return true;
    }
    return false;
}

bool Reservation::isActive() const {
    return (status == ReservationStatus::CONFIRMED || status == ReservationStatus::IN_PROGRESS);
}
