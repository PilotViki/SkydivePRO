#ifndef PLANE_SLOT_H
#define PLANE_SLOT_H

#include <string>
#include <ctime>

class PlaneSlot {
private:
    int slotID;
    std::time_t departureTime;
    int capacity;
    int reservedSeats;
    double pricePerSeat;
    int altitude;  // in feet
    bool active;   // If the flight is still available or has been cancelled
    static int nextSlotID;

public:
    // Constructors
    PlaneSlot();
    PlaneSlot(std::time_t departureTime, int capacity, double pricePerSeat, int altitude);
    
    // Getters
    int getSlotID() const;
    std::time_t getDepartureTime() const;
    int getCapacity() const;
    int getReservedSeats() const;
    int getAvailableSeats() const;
    double getPricePerSeat() const;
    int getAltitude() const;
    bool isActive() const;
    
    // Setters
    void setDepartureTime(std::time_t time);
    void setCapacity(int capacity);
    void setPricePerSeat(double price);
    void setAltitude(int altitude);
    void setActive(bool active);
    
    // Operations
    bool reserveSeat();
    bool cancelSeatReservation();
    bool isFull() const;
    
    // Utility
    std::string getFormattedDepartureTime() const;
};

#endif // PLANE_SLOT_H
