#include "../include/Client.h"

Client::Client() : User(), loyaltyPoints(0) {
}

Client::Client(const std::string& name, const std::string& email, const std::string& password)
    : User(name, email, password), loyaltyPoints(0) {
}

Client::~Client() {
    // Specific client cleanup if needed
}

std::string Client::getRole() const {
    return "Client";
}

int Client::getLoyaltyPoints() const {
    return loyaltyPoints;
}

void Client::addLoyaltyPoints(int points) {
    if (points > 0) {
        loyaltyPoints += points;
    }
}

void Client::useLoyaltyPoints(int points) {
    if (points > 0 && points <= loyaltyPoints) {
        loyaltyPoints -= points;
    }
}

void Client::addReservation(int reservationID) {
    reservationIDs.push_back(reservationID);
}

void Client::removeReservation(int reservationID) {
    for (auto it = reservationIDs.begin(); it != reservationIDs.end(); ++it) {
        if (*it == reservationID) {
            reservationIDs.erase(it);
            break;
        }
    }
}

const std::vector<int>& Client::getReservationIDs() const {
    return reservationIDs;
}

double Client::calculateAvailableDiscount() const {
    // Example: every 10 points = 25 lei discount
    return (loyaltyPoints / 10) * 25.0;
}
