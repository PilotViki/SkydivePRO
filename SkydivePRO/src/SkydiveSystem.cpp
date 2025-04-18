#include "../include/SkydiveSystem.h"
#include <fstream>
#include <algorithm>

// Adăugare implementări goale pentru funcțiile de prevenire a copierii
SkydiveSystem::SkydiveSystem(const SkydiveSystem&) {}
SkydiveSystem& SkydiveSystem::operator=(const SkydiveSystem&) { return *this; }

// Initialize static instance pointer
SkydiveSystem* SkydiveSystem::instance = nullptr;

SkydiveSystem::SkydiveSystem() {
    // Initialize the system
}

SkydiveSystem* SkydiveSystem::getInstance() {
    if (instance == nullptr) {
        instance = new SkydiveSystem();
    }
    return instance;
}

bool SkydiveSystem::registerClient(const std::string& name, const std::string& email, const std::string& password) {
    // Check if email already exists
    for (const auto& user_pair : users) {
        if (user_pair.second->getEmail() == email) {
            return false; // Email already registered
        }
    }
    
    // Create new client
    std::shared_ptr<User> newClient = std::make_shared<Client>(name, email, password);
    users[newClient->getUserID()] = newClient;
    
    return true;
}

bool SkydiveSystem::registerAdmin(const std::string& name, const std::string& email, const std::string& password, 
                               const std::string& accessLevel) {
    // Check if email already exists
    for (const auto& user_pair : users) {
        if (user_pair.second->getEmail() == email) {
            return false; // Email already registered
        }
    }
    
    // Create new admin
    std::shared_ptr<User> newAdmin = std::make_shared<Administrator>(name, email, password, accessLevel);
    users[newAdmin->getUserID()] = newAdmin;
    
    return true;
}

bool SkydiveSystem::login(const std::string& email, const std::string& password) {
    for (const auto& user_pair : users) {
        if (user_pair.second->authenticate(email, password)) {
            currentUser = user_pair.second;
            return true;
        }
    }
    return false;
}

void SkydiveSystem::logout() {
    currentUser = nullptr;
}

std::shared_ptr<User> SkydiveSystem::getCurrentUser() const {
    return currentUser;
}

bool SkydiveSystem::addEquipment(EquipmentType type, const std::string& size, double pricePerDay, int loyaltyPoints) {
    // Create new equipment
    Equipment newEquipment(type, size, pricePerDay, loyaltyPoints);
    equipment[newEquipment.getEquipmentID()] = newEquipment;
    
    return true;
}

bool SkydiveSystem::updateEquipment(int equipmentID, const std::string& size, double pricePerDay, bool available) {
    auto it = equipment.find(equipmentID);
    if (it == equipment.end()) {
        return false; // Equipment not found
    }
    
    it->second.setSize(size);
    it->second.setPricePerDay(pricePerDay);
    it->second.setAvailability(available);
    
    return true;
}

bool SkydiveSystem::removeEquipment(int equipmentID) {
    return equipment.erase(equipmentID) > 0;
}

Equipment* SkydiveSystem::getEquipment(int equipmentID) {
    auto it = equipment.find(equipmentID);
    if (it != equipment.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<Equipment*> SkydiveSystem::getAvailableEquipment(EquipmentType type, const std::string& size) {
    std::vector<Equipment*> availableEquipment;
    
    for (auto& equip_pair : equipment) {
        Equipment& equip = equip_pair.second;
        
        // Check if equipment matches criteria and is available
        if (equip.isAvailable() && 
            (type == EquipmentType::RIG || equip.getType() == type) &&
            (size.empty() || equip.getSize() == size)) {
            
            availableEquipment.push_back(&equip);
        }
    }
    
    return availableEquipment;
}

bool SkydiveSystem::addPlaneSlot(std::time_t departureTime, int capacity, double pricePerSeat, int altitude) {
    // Create new plane slot
    PlaneSlot newSlot(departureTime, capacity, pricePerSeat, altitude);
    planeSlots[newSlot.getSlotID()] = newSlot;
    
    return true;
}

bool SkydiveSystem::updatePlaneSlot(int slotID, std::time_t departureTime, double pricePerSeat, bool active) {
    auto it = planeSlots.find(slotID);
    if (it == planeSlots.end()) {
        return false; // Slot not found
    }
    
    it->second.setDepartureTime(departureTime);
    it->second.setPricePerSeat(pricePerSeat);
    it->second.setActive(active);
    
    return true;
}

bool SkydiveSystem::removePlaneSlot(int slotID) {
    return planeSlots.erase(slotID) > 0;
}

PlaneSlot* SkydiveSystem::getPlaneSlot(int slotID) {
    auto it = planeSlots.find(slotID);
    if (it != planeSlots.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<PlaneSlot*> SkydiveSystem::getAvailablePlaneSlots(std::time_t startDate, std::time_t endDate) {
    std::vector<PlaneSlot*> availableSlots;
    
    for (auto& slot_pair : planeSlots) {
        PlaneSlot& slot = slot_pair.second;
        
        // Check if slot is active, has available seats, and is within date range
        if (slot.isActive() && 
            !slot.isFull() && 
            slot.getDepartureTime() >= startDate && 
            slot.getDepartureTime() <= endDate) {
            
            availableSlots.push_back(&slot);
        }
    }
    
    return availableSlots;
}

int SkydiveSystem::createReservation(int clientID, const std::vector<int>& equipmentIDs, int planeSlotID, 
                                   std::time_t reservationDate) {
    // Validate client exists
    bool clientExists = false;
    for (const auto& user_pair : users) {
        if (user_pair.first == clientID && user_pair.second->getRole() == "Client") {
            clientExists = true;
            break;
        }
    }
    
    if (!clientExists) {
        return -1; // Invalid client
    }
    
    // Validate equipment availability
    for (int equipID : equipmentIDs) {
        auto equip = getEquipment(equipID);
        if (!equip || !equip->isAvailable()) {
            return -1; // Equipment not available
        }
    }
    
    // Validate plane slot
    if (planeSlotID != -1) {
        auto slot = getPlaneSlot(planeSlotID);
        if (!slot || !slot->isActive() || slot->isFull()) {
            return -1; // Slot not available
        }
    }
    
    // Create the reservation
    Reservation newReservation(clientID, equipmentIDs, planeSlotID, reservationDate);
    
    // Calculate total cost
    double totalCost = 0.0;
    int loyaltyPoints = 0;
    
    // Add equipment costs
    for (int equipID : equipmentIDs) {
        auto equip = getEquipment(equipID);
        if (equip) {
            totalCost += equip->getPricePerDay();
            loyaltyPoints += equip->getLoyaltyPointsAwarded();
            equip->setAvailability(false); // Mark as unavailable
        }
    }
    
    // Add plane slot cost
    if (planeSlotID != -1) {
        auto slot = getPlaneSlot(planeSlotID);
        if (slot) {
            totalCost += slot->getPricePerSeat();
            slot->reserveSeat(); // Reserve a seat
        }
    }
    
    newReservation.setTotalCost(totalCost);
    newReservation.setLoyaltyPointsEarned(loyaltyPoints);
    
    // Add to reservations
    int reservationID = newReservation.getReservationID();
    reservations[reservationID] = newReservation;
    
    // Add to client's reservation list
    auto user = dynamic_cast<Client*>(users[clientID].get());
    if (user) {
        user->addReservation(reservationID);
    }
    
    // Add loyalty points
    LoyaltySystem::getInstance()->addPoints(clientID, 
        LoyaltySystem::getInstance()->calculatePointsFromPurchase(totalCost));
    
    return reservationID;
}

bool SkydiveSystem::updateReservationStatus(int reservationID, ReservationStatus status) {
    auto it = reservations.find(reservationID);
    if (it == reservations.end()) {
        return false; // Reservation not found
    }
    
    // Update the status
    return it->second.updateStatus(status);
}

bool SkydiveSystem::cancelReservation(int reservationID) {
    auto it = reservations.find(reservationID);
    if (it == reservations.end()) {
        return false; // Reservation not found
    }
    
    // If already cancelled, do nothing
    if (it->second.getStatus() == ReservationStatus::CANCELLED) {
        return true;
    }
    
    // Cancel the reservation
    if (!it->second.cancelReservation()) {
        return false;
    }
    
    // Free up equipment
    for (int equipID : it->second.getEquipmentIDs()) {
        auto equip = getEquipment(equipID);
        if (equip) {
            equip->setAvailability(true);
        }
    }
    
    // Free up plane slot
    int slotID = it->second.getPlaneSlotID();
    if (slotID != -1) {
        auto slot = getPlaneSlot(slotID);
        if (slot) {
            slot->cancelSeatReservation();
        }
    }
    
    // Remove from client's reservation list
    int clientID = it->second.getClientID();
    auto user = dynamic_cast<Client*>(users[clientID].get());
    if (user) {
        user->removeReservation(reservationID);
    }
    
    return true;
}

Reservation* SkydiveSystem::getReservation(int reservationID) {
    auto it = reservations.find(reservationID);
    if (it != reservations.end()) {
        return &(it->second);
    }
    return nullptr;
}

std::vector<Reservation*> SkydiveSystem::getClientReservations(int clientID) {
    std::vector<Reservation*> clientReservations;
    
    for (auto& res_pair : reservations) {
        if (res_pair.second.getClientID() == clientID) {
            clientReservations.push_back(&(res_pair.second));
        }
    }
    
    return clientReservations;
}

std::vector<Reservation*> SkydiveSystem::getReservationsInPeriod(std::time_t startDate, std::time_t endDate) {
    std::vector<Reservation*> periodReservations;
    
    for (auto& res_pair : reservations) {
        Reservation& res = res_pair.second;
        std::time_t resDate = res.getReservationDate();
        
        if (resDate >= startDate && resDate <= endDate) {
            periodReservations.push_back(&res);
        }
    }
    
    return periodReservations;
}

double SkydiveSystem::calculateTotalRevenue(std::time_t startDate, std::time_t endDate) {
    double totalRevenue = 0.0;
    
    auto periodReservations = getReservationsInPeriod(startDate, endDate);
    for (const auto* res : periodReservations) {
        // Only count confirmed or completed reservations
        if (res->getStatus() == ReservationStatus::CONFIRMED || 
            res->getStatus() == ReservationStatus::COMPLETED) {
            totalRevenue += res->getTotalCost();
        }
    }
    
    return totalRevenue;
}

std::map<EquipmentType, int> SkydiveSystem::getEquipmentUsageStats() {
    std::map<EquipmentType, int> usageStats;
    
    // Initialize counts for all equipment types
    usageStats[EquipmentType::RIG] = 0;
    usageStats[EquipmentType::ALTIMETER] = 0;
    usageStats[EquipmentType::HELMET] = 0;
    usageStats[EquipmentType::JUMPSUIT] = 0;
    
    // Count equipment usage in active reservations
    for (const auto& res_pair : reservations) {
        const Reservation& res = res_pair.second;
        
        // Only count confirmed or in-progress reservations
        if (res.getStatus() != ReservationStatus::CONFIRMED && 
            res.getStatus() != ReservationStatus::IN_PROGRESS) {
            continue;
        }
        
        // Count each equipment
        for (int equipID : res.getEquipmentIDs()) {
            auto equip = getEquipment(equipID);
            if (equip) {
                usageStats[equip->getType()]++;
            }
        }
    }
    
    return usageStats;
}

bool SkydiveSystem::saveData() const {
    try {
        // Implementation for saving data to files would go here
        // For simplicity, we're not implementing the actual file I/O
        return true;
    } catch (...) {
        return false;
    }
}

bool SkydiveSystem::loadData() {
    try {
        // Pre-populate the system with sample data for a realistic experience
        
        // Add example administrators
        if (users.empty()) {
            registerAdmin("Admin Principal", "admin@skydive.com", "admin123", "full");
            registerAdmin("Responsabil Echipamente", "echipamente@skydive.com", "echip123", "equipment-only");
            registerAdmin("Planificator Zboruri", "zboruri@skydive.com", "zbor123", "reservation-only");
            
            // Add example clients
            registerClient("Andrei Popescu", "andrei@example.com", "client123");
            registerClient("Maria Ionescu", "maria@example.com", "client123");
            registerClient("Alex Dumitrescu", "alex@example.com", "client123");
        }
        
        // Only add equipment if there isn't any already
        if (equipment.empty()) {
            // PARAȘUTE DE ȘCOALĂ (RIG)
            addEquipment(EquipmentType::RIG, "Student Solo XL (>90kg)", 75.0, 2); // Size, Price/Day, Loyalty Points
            addEquipment(EquipmentType::RIG, "Student Solo L (80-90kg)", 75.0, 2);
            addEquipment(EquipmentType::RIG, "Student Solo M (70-80kg)", 75.0, 2);
            addEquipment(EquipmentType::RIG, "Student Solo S (<70kg)", 75.0, 2);
            addEquipment(EquipmentType::RIG, "Student Tandem XL", 120.0, 3);
            
            // PARAȘUTE DE PERFORMANȚĂ (RIG)
            addEquipment(EquipmentType::RIG, "Javelin Odyssey XL", 95.0, 3);
            addEquipment(EquipmentType::RIG, "Javelin Odyssey L", 95.0, 3);
            addEquipment(EquipmentType::RIG, "Vector 3 M", 95.0, 3);
            addEquipment(EquipmentType::RIG, "Vector 3 S", 95.0, 3);
            addEquipment(EquipmentType::RIG, "Wings XS (Competiție)", 120.0, 4);
            
            // ALTIMETRE
            addEquipment(EquipmentType::ALTIMETER, "Altimetru Digital Viso II", 15.0, 1);
            addEquipment(EquipmentType::ALTIMETER, "Altimetru Digital Viso II+", 20.0, 1);
            addEquipment(EquipmentType::ALTIMETER, "Altimetru Analogic L&B", 10.0, 1);
            addEquipment(EquipmentType::ALTIMETER, "Altimetru Analogic FT-50", 10.0, 1);
            addEquipment(EquipmentType::ALTIMETER, "Altimetru Sonor Quattro", 25.0, 1);
            
            // CĂȘTI
            addEquipment(EquipmentType::HELMET, "Cookie G4 XL", 15.0, 1);
            addEquipment(EquipmentType::HELMET, "Cookie G4 L", 15.0, 1);
            addEquipment(EquipmentType::HELMET, "Cookie G4 M", 15.0, 1);
            addEquipment(EquipmentType::HELMET, "Cookie G4 S", 15.0, 1);
            addEquipment(EquipmentType::HELMET, "Cookie Fuel XL", 20.0, 1);
            addEquipment(EquipmentType::HELMET, "Cookie Fuel L", 20.0, 1);
            addEquipment(EquipmentType::HELMET, "Cookie Fuel M", 20.0, 1);
            addEquipment(EquipmentType::HELMET, "Cookie Fuel S", 20.0, 1);
            
            // COMBINEZOANE
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Începători XL", 20.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Începători L", 20.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Începători M", 20.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Începători S", 20.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Freefly XL", 30.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Freefly L", 30.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Freefly M", 30.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Freefly S", 30.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Tracking XL", 35.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Combinezon Tracking L", 35.0, 1);
            addEquipment(EquipmentType::JUMPSUIT, "Wingsuit Beginner L", 45.0, 2);
            addEquipment(EquipmentType::JUMPSUIT, "Wingsuit Beginner M", 45.0, 2);
        }
        
        // Add plane slots for the next 7 days if none exist
        if (planeSlots.empty()) {
            std::time_t now = std::time(nullptr);
            std::tm* localTime = std::localtime(&now);
            
            // Create slots for the next 7 days
            for (int day = 0; day < 7; day++) {
                // Set time to the beginning of the current day + 'day' days
                localTime->tm_hour = 0;
                localTime->tm_min = 0;
                localTime->tm_sec = 0;
                localTime->tm_mday += (day == 0 ? 0 : 1); // Only add a day if it's not the first iteration
                std::mktime(localTime); // Update struct tm
                
                // Morning flight (10:00) - 4000m
                localTime->tm_hour = 10;
                std::time_t morningTime = std::mktime(localTime);
                addPlaneSlot(morningTime, 15, 180.0, 4000); // 15 seats, 180 lei, 4000m
                
                // Noon flight (13:00) - 3000m
                localTime->tm_hour = 13;
                std::time_t noonTime = std::mktime(localTime);
                addPlaneSlot(noonTime, 15, 150.0, 3000); // 15 seats, 150 lei, 3000m
                
                // Afternoon flight (16:00) - 4000m
                localTime->tm_hour = 16;
                std::time_t afternoonTime = std::mktime(localTime);
                addPlaneSlot(afternoonTime, 15, 180.0, 4000); // 15 seats, 180 lei, 4000m
                
                // Sunset flight (19:00) - 2500m (only on weekends - Friday, Saturday, Sunday)
                if (localTime->tm_wday == 5 || localTime->tm_wday == 6 || localTime->tm_wday == 0) {
                    localTime->tm_hour = 19;
                    std::time_t sunsetTime = std::mktime(localTime);
                    addPlaneSlot(sunsetTime, 10, 130.0, 2500); // 10 seats, 130 lei, 2500m
                }
            }
        }
        
        // Configure loyalty system
        LoyaltySystem::getInstance()->setMoneyToPointsRatio(50.0); // 1 punct la fiecare 50 lei
        LoyaltySystem::getInstance()->setPointsToMoneyRatio(10.0); // 10 puncte = 1 leu reducere
        
        return true;
    } catch (...) {
        return false;
    }
}
