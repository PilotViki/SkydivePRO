#include "../include/PlaneSlot.h"
#include <ctime>
#include <sstream>
#include <iomanip>

// Initialize static member
int PlaneSlot::nextSlotID = 1;

PlaneSlot::PlaneSlot()
    : slotID(0),
      departureTime(0),
      capacity(0),
      reservedSeats(0),
      pricePerSeat(0.0),
      altitude(0),
      active(false) {
}

PlaneSlot::PlaneSlot(std::time_t departureTime, int capacity, double pricePerSeat, int altitude)
    : slotID(nextSlotID++),
      departureTime(departureTime),
      capacity(capacity),
      reservedSeats(0),
      pricePerSeat(pricePerSeat),
      altitude(altitude),
      active(true) {
}

int PlaneSlot::getSlotID() const {
    return slotID;
}

std::time_t PlaneSlot::getDepartureTime() const {
    return departureTime;
}

int PlaneSlot::getCapacity() const {
    return capacity;
}

int PlaneSlot::getReservedSeats() const {
    return reservedSeats;
}

int PlaneSlot::getAvailableSeats() const {
    return capacity - reservedSeats;
}

double PlaneSlot::getPricePerSeat() const {
    return pricePerSeat;
}

int PlaneSlot::getAltitude() const {
    return altitude;
}

bool PlaneSlot::isActive() const {
    return active;
}

void PlaneSlot::setDepartureTime(std::time_t time) {
    departureTime = time;
}

void PlaneSlot::setCapacity(int capacity) {
    if (capacity >= reservedSeats) {
        this->capacity = capacity;
    }
}

void PlaneSlot::setPricePerSeat(double price) {
    if (price >= 0) {
        pricePerSeat = price;
    }
}

void PlaneSlot::setAltitude(int altitude) {
    if (altitude > 0) {
        this->altitude = altitude;
    }
}

void PlaneSlot::setActive(bool active) {
    this->active = active;
}

bool PlaneSlot::reserveSeat() {
    if (active && reservedSeats < capacity) {
        reservedSeats++;
        return true;
    }
    return false;
}

bool PlaneSlot::cancelSeatReservation() {
    if (reservedSeats > 0) {
        reservedSeats--;
        return true;
    }
    return false;
}

bool PlaneSlot::isFull() const {
    return reservedSeats >= capacity;
}

std::string PlaneSlot::getFormattedDepartureTime() const {
    std::tm* localTime = std::localtime(&departureTime);
    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M");
    return oss.str();
}
