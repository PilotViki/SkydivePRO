#ifndef CLIENT_H
#define CLIENT_H

#include "User.h"
#include <vector>

// Forward declarations
class Reservation;

class Client : public User {
private:
    int loyaltyPoints;
    std::vector<int> reservationIDs;  // Store IDs of reservations made by this client

public:
    // Constructors
    Client();
    Client(const std::string& name, const std::string& email, const std::string& password);
    
    // Destructor
    ~Client();
    
    // Implementation of the pure virtual method from User
    std::string getRole() const;
    
    // Loyalty points methods
    int getLoyaltyPoints() const;
    void addLoyaltyPoints(int points);
    void useLoyaltyPoints(int points);
    
    // Reservation-related methods
    void addReservation(int reservationID);
    void removeReservation(int reservationID);
    const std::vector<int>& getReservationIDs() const;
    
    // Calculate available discount based on loyalty points
    double calculateAvailableDiscount() const;
};

#endif // CLIENT_H
