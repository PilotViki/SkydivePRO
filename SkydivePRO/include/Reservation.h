#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <vector>
#include <ctime>

// Enum pentru status-ul rezervării (C++98 compatible)
enum ReservationStatus {
    PENDING,
    CONFIRMED,
    CANCELLED,
    IN_PROGRESS,
    COMPLETED
};

class Reservation {
private:
    int reservationID;
    int clientID;
    std::vector<int> equipmentIDs;
    int planeSlotID;  // -1 if no plane slot is reserved
    std::time_t creationDate;
    std::time_t reservationDate;
    ReservationStatus status;
    double totalCost;
    int loyaltyPointsEarned;
    static int nextReservationID;

public:
    // Constructors
    Reservation();
    Reservation(int clientID, const std::vector<int>& equipmentIDs, int planeSlotID, 
                std::time_t reservationDate);
    
    // Getters
    int getReservationID() const;
    int getClientID() const;
    const std::vector<int>& getEquipmentIDs() const;
    int getPlaneSlotID() const;
    std::time_t getCreationDate() const;
    std::time_t getReservationDate() const;
    ReservationStatus getStatus() const;
    std::string getStatusAsString() const;
    double getTotalCost() const;
    int getLoyaltyPointsEarned() const;
    
    // Setters
    void setClientID(int clientID);
    void addEquipment(int equipmentID);
    void removeEquipment(int equipmentID);
    void setPlaneSlotID(int slotID);
    void setReservationDate(std::time_t date);
    void setStatus(ReservationStatus status);
    void setTotalCost(double cost);
    void setLoyaltyPointsEarned(int points);
    
    // Utility methods
    bool updateStatus(ReservationStatus newStatus);
    bool cancelReservation();
    bool isActive() const;  // Returns true if status is CONFIRMED or IN_PROGRESS
};

#endif // RESERVATION_H
