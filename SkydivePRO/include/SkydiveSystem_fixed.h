#ifndef SKYDIVE_SYSTEM_H
#define SKYDIVE_SYSTEM_H

#include "User.h"
#include "Client.h"
#include "Administrator.h"
#include "Equipment.h"
#include "Reservation.h"
#include "PlaneSlot.h"
#include "LoyaltySystem.h"

#include <vector>
#include <map>
#include <memory>

class SkydiveSystem {
private:
    // Singleton pattern
    static SkydiveSystem* instance;
    
    // Collections
    std::map<int, std::shared_ptr<User> > users;
    std::map<int, Equipment> equipment;
    std::map<int, Reservation> reservations;
    std::map<int, PlaneSlot> planeSlots;
    
    // Active user session
    std::shared_ptr<User> currentUser;
    
    // Private constructor (Singleton pattern)
    SkydiveSystem();
    
public:
    // Singleton accessor
    static SkydiveSystem* getInstance();
    
    // Prevent copying (utilizează C++98 compatibil cu C++11)
    SkydiveSystem(const SkydiveSystem&); // nu este implementată
    SkydiveSystem& operator=(const SkydiveSystem&); // nu este implementată
    
    // User management
    bool registerClient(const std::string& name, const std::string& email, const std::string& password);
    bool registerAdmin(const std::string& name, const std::string& email, const std::string& password, 
                       const std::string& accessLevel = "full");
    bool login(const std::string& email, const std::string& password);
    void logout();
    std::shared_ptr<User> getCurrentUser() const;
    
    // Equipment management
    bool addEquipment(EquipmentType type, const std::string& size, double pricePerDay, int loyaltyPoints);
    bool updateEquipment(int equipmentID, const std::string& size, double pricePerDay, bool available);
    bool removeEquipment(int equipmentID);
    Equipment* getEquipment(int equipmentID);
    std::vector<Equipment*> getAvailableEquipment(EquipmentType type, const std::string& size = "");
    
    // Plane slot management
    bool addPlaneSlot(std::time_t departureTime, int capacity, double pricePerSeat, int altitude);
    bool updatePlaneSlot(int slotID, std::time_t departureTime, double pricePerSeat, bool active);
    bool removePlaneSlot(int slotID);
    PlaneSlot* getPlaneSlot(int slotID);
    std::vector<PlaneSlot*> getAvailablePlaneSlots(std::time_t startDate, std::time_t endDate);
    
    // Reservation management
    int createReservation(int clientID, const std::vector<int>& equipmentIDs, int planeSlotID, 
                        std::time_t reservationDate);
    bool updateReservationStatus(int reservationID, ReservationStatus status);
    bool cancelReservation(int reservationID);
    Reservation* getReservation(int reservationID);
    std::vector<Reservation*> getClientReservations(int clientID);
    
    // System reports
    std::vector<Reservation*> getReservationsInPeriod(std::time_t startDate, std::time_t endDate);
    double calculateTotalRevenue(std::time_t startDate, std::time_t endDate);
    std::map<EquipmentType, int> getEquipmentUsageStats();
    
    // Save/Load data from files
    bool saveData() const;
    bool loadData();
};

#endif // SKYDIVE_SYSTEM_H
