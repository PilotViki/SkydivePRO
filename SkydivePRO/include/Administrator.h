#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"

class Administrator : public User {
private:
    std::string accessLevel;  // Could be "full", "equipment-only", "reservation-only", etc.

public:
    // Constructors
    Administrator();
    Administrator(const std::string& name, const std::string& email, const std::string& password, 
                  const std::string& accessLevel = "full");
    
    // Destructor
    ~Administrator();
    
    // Implementation of the pure virtual method from User
    std::string getRole() const;
    
    // Admin-specific methods
    std::string getAccessLevel() const;
    void setAccessLevel(const std::string& level);
    
    // Admin operations (these would typically interact with the system)
    bool confirmReservation(int reservationID);
    bool cancelReservation(int reservationID);
    bool addEquipment(const std::string& type, const std::string& size, double price);
    bool updateEquipmentStatus(int equipmentID, const std::string& status);
};

#endif // ADMINISTRATOR_H
