#include "../include/SkydiveSystem.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>

// Function prototypes
void displayMainMenu();
void displayClientMenu();
void displayAdminMenu();
void loginProcess();
void registerProcess();
void displayEquipment(const std::vector<Equipment*>& equipmentList);
void displayPlaneSlots(const std::vector<PlaneSlot*>& slotList);
void handleClientReservation();
void viewClientReservations();
void handleAdminEquipment();
void handleAdminPlaneSlots();
void viewAdminReservations();
void viewSystemStatistics();

int main() {
    // Initialize the system
    SkydiveSystem* system = SkydiveSystem::getInstance();
    system->loadData();
    
    // Initial setup - creating one admin if none exists
    if (system->login("admin@skydive.com", "admin123")) {
        system->logout();
    } else {
        system->registerAdmin("Admin", "admin@skydive.com", "admin123", "full");
    }
    
    // Main program loop
    bool running = true;
    char choice;
    
    while (running) {
        displayMainMenu();
        std::cin >> choice;
        
        switch (choice) {
            case '1':
                loginProcess();
                break;
            case '2':
                registerProcess();
                break;
            case '3':
                std::cout << "\nThank you for using SkydivePRO! Goodbye!\n";
                running = false;
                break;
            default:
                std::cout << "\nInvalid choice. Please try again.\n";
                break;
        }
    }
    
    // Save data before exit
    system->saveData();
    
    return 0;
}

void displayMainMenu() {
    std::cout << "\n===== Welcome to SkydivePRO =====\n";
    std::cout << "1. Login\n";
    std::cout << "2. Register\n";
    std::cout << "3. Exit\n";
    std::cout << "Please enter your choice: ";
}

void displayClientMenu() {
    std::cout << "\n===== Client Menu =====\n";
    std::cout << "1. View Available Equipment\n";
    std::cout << "2. View Available Plane Slots\n";
    std::cout << "3. Make a Reservation\n";
    std::cout << "4. View My Reservations\n";
    std::cout << "5. View My Loyalty Points\n";
    std::cout << "6. Logout\n";
    std::cout << "Please enter your choice: ";
}

void displayAdminMenu() {
    std::cout << "\n===== Administrator Menu =====\n";
    std::cout << "1. Manage Equipment\n";
    std::cout << "2. Manage Plane Slots\n";
    std::cout << "3. View All Reservations\n";
    std::cout << "4. System Statistics\n";
    std::cout << "5. Logout\n";
    std::cout << "Please enter your choice: ";
}

void loginProcess() {
    std::string email, password;
    SkydiveSystem* system = SkydiveSystem::getInstance();
    
    std::cout << "\n===== Login =====\n";
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;
    
    if (system->login(email, password)) {
        std::cout << "\nLogin successful! Welcome, " << system->getCurrentUser()->getName() << "!\n";
        
        // Determine user role and display appropriate menu
        if (system->getCurrentUser()->getRole() == "Client") {
            bool clientLoggedIn = true;
            char clientChoice;
            
            while (clientLoggedIn) {
                displayClientMenu();
                std::cin >> clientChoice;
                
                switch (clientChoice) {
                    case '1': {
                        // View Available Equipment - toate tipurile
                        auto equipment = system->getAvailableEquipment(RIG, "");
                        displayEquipment(equipment);
                        break;
                    }
                    case '2': {
                        // View Available Plane Slots
                        std::time_t now = std::time(nullptr);
                        std::time_t oneMonthLater = now + (30 * 24 * 60 * 60);
                        auto slots = system->getAvailablePlaneSlots(now, oneMonthLater);
                        displayPlaneSlots(slots);
                        break;
                    }
                    case '3':
                        // Make a Reservation
                        handleClientReservation();
                        break;
                    case '4':
                        // View My Reservations
                        viewClientReservations();
                        break;
                    case '5': {
                        // View My Loyalty Points
                        auto client = dynamic_cast<Client*>(system->getCurrentUser().get());
                        if (client) {
                            std::cout << "\nYou have " << client->getLoyaltyPoints() 
                                    << " loyalty points, which can provide up to " 
                                    << client->calculateAvailableDiscount() 
                                    << " lei in discounts.\n";
                        }
                        break;
                    }
                    case '6':
                        // Logout
                        system->logout();
                        clientLoggedIn = false;
                        std::cout << "\nYou have been logged out.\n";
                        break;
                    default:
                        std::cout << "\nInvalid choice. Please try again.\n";
                        break;
                }
            }
        } 
        else if (system->getCurrentUser()->getRole() == "Administrator") {
            bool adminLoggedIn = true;
            char adminChoice;
            
            while (adminLoggedIn) {
                displayAdminMenu();
                std::cin >> adminChoice;
                
                switch (adminChoice) {
                    case '1':
                        // Manage Equipment
                        handleAdminEquipment();
                        break;
                    case '2':
                        // Manage Plane Slots
                        handleAdminPlaneSlots();
                        break;
                    case '3':
                        // View All Reservations
                        viewAdminReservations();
                        break;
                    case '4':
                        // System Statistics
                        viewSystemStatistics();
                        break;
                    case '5':
                        // Logout
                        system->logout();
                        adminLoggedIn = false;
                        std::cout << "\nYou have been logged out.\n";
                        break;
                    default:
                        std::cout << "\nInvalid choice. Please try again.\n";
                        break;
                }
            }
        }
    } else {
        std::cout << "\nLogin failed. Invalid email or password.\n";
    }
}

void registerProcess() {
    std::string name, email, password;
    SkydiveSystem* system = SkydiveSystem::getInstance();
    
    std::cout << "\n===== Register as a New Client =====\n";
    std::cout << "Name: ";
    std::cin.ignore();
    std::getline(std::cin, name);
    std::cout << "Email: ";
    std::cin >> email;
    std::cout << "Password: ";
    std::cin >> password;
    
    if (system->registerClient(name, email, password)) {
        std::cout << "\nRegistration successful! You can now login.\n";
    } else {
        std::cout << "\nRegistration failed. Email may already be in use.\n";
    }
}

void displayEquipment(const std::vector<Equipment*>& equipmentList) {
    if (equipmentList.empty()) {
        std::cout << "\nNo equipment available at the moment.\n";
        return;
    }
    
    std::cout << "\n===== Available Equipment =====\n";
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(12) << "Type" 
              << std::setw(12) << "Size" 
              << std::setw(15) << "Price/Day"
              << "Loyalty Points\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (const auto* equip : equipmentList) {
        std::cout << std::left << std::setw(5) << equip->getEquipmentID()
                  << std::setw(12) << equip->getTypeAsString()
                  << std::setw(12) << equip->getSize()
                  << std::setw(15) << equip->getPricePerDay()
                  << equip->getLoyaltyPointsAwarded() << "\n";
    }
}

void displayPlaneSlots(const std::vector<PlaneSlot*>& slotList) {
    if (slotList.empty()) {
        std::cout << "\nNo plane slots available at the moment.\n";
        return;
    }
    
    std::cout << "\n===== Available Plane Slots =====\n";
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(20) << "Departure Time" 
              << std::setw(12) << "Altitude" 
              << std::setw(12) << "Price"
              << "Available Seats\n";
    std::cout << std::string(60, '-') << "\n";
    
    for (const auto* slot : slotList) {
        std::cout << std::left << std::setw(5) << slot->getSlotID()
                  << std::setw(20) << slot->getFormattedDepartureTime()
                  << std::setw(12) << slot->getAltitude()
                  << std::setw(12) << slot->getPricePerSeat()
                  << slot->getAvailableSeats() << "\n";
    }
}

void handleClientReservation() {
    SkydiveSystem* system = SkydiveSystem::getInstance();
    auto client = dynamic_cast<Client*>(system->getCurrentUser().get());
    
    if (!client) {
        std::cout << "\nError: You must be logged in as a client to make reservations.\n";
        return;
    }
    
    int clientID = client->getUserID();
    std::vector<int> selectedEquipment;
    int planeSlotID = -1;
    std::time_t reservationDate = std::time(nullptr); // Current time as default
    
    // Step 1: Select equipment
    std::cout << "\n===== Make Reservation: Step 1 - Equipment Selection =====\n";
    auto availableEquipment = system->getAvailableEquipment(RIG, "");
    displayEquipment(availableEquipment);
    
    std::string moreEquipment = "yes";
    while (moreEquipment == "yes") {
        int equipID;
        std::cout << "\nEnter equipment ID to select (0 to skip): ";
        std::cin >> equipID;
        
        if (equipID == 0) {
            break;
        }
        
        Equipment* equip = system->getEquipment(equipID);
        if (equip && equip->isAvailable()) {
            selectedEquipment.push_back(equipID);
            std::cout << "Equipment added to your selection.\n";
        } else {
            std::cout << "Invalid equipment ID or equipment not available.\n";
        }
        
        std::cout << "Add more equipment? (yes/no): ";
        std::cin >> moreEquipment;
    }
    
    // Step 2: Select plane slot (optional)
    std::cout << "\n===== Make Reservation: Step 2 - Plane Slot Selection =====\n";
    std::cout << "Do you want to reserve a plane slot? (yes/no): ";
    std::string wantPlaneSlot;
    std::cin >> wantPlaneSlot;
    
    if (wantPlaneSlot == "yes") {
        std::time_t now = std::time(nullptr);
        std::time_t oneMonthLater = now + (30 * 24 * 60 * 60);
        auto availableSlots = system->getAvailablePlaneSlots(now, oneMonthLater);
        displayPlaneSlots(availableSlots);
        
        std::cout << "\nEnter plane slot ID to select (0 to skip): ";
        std::cin >> planeSlotID;
        
        if (planeSlotID > 0) {
            auto slot = system->getPlaneSlot(planeSlotID);
            if (!slot || !slot->isActive() || slot->isFull()) {
                std::cout << "Invalid slot ID or slot not available. Skipping slot reservation.\n";
                planeSlotID = -1;
            } else {
                reservationDate = slot->getDepartureTime();
            }
        } else {
            planeSlotID = -1;
        }
    }
    
    // Step 3: Confirm reservation
    if (selectedEquipment.empty() && planeSlotID == -1) {
        std::cout << "\nYou haven't selected any equipment or plane slot. Reservation cancelled.\n";
        return;
    }
    
    std::cout << "\n===== Make Reservation: Step 3 - Confirmation =====\n";
    std::cout << "Selected Equipment IDs: ";
    for (int id : selectedEquipment) {
        std::cout << id << " ";
    }
    std::cout << "\nSelected Plane Slot ID: " << (planeSlotID == -1 ? "None" : std::to_string(planeSlotID)) << "\n";
    
    std::cout << "\nConfirm reservation? (yes/no): ";
    std::string confirm;
    std::cin >> confirm;
    
    if (confirm == "yes") {
        int reservationID = system->createReservation(clientID, selectedEquipment, planeSlotID, reservationDate);
        
        if (reservationID > 0) {
            std::cout << "\nReservation created successfully! Your reservation ID is: " << reservationID << "\n";
        } else {
            std::cout << "\nFailed to create reservation. Please try again.\n";
        }
    } else {
        std::cout << "\nReservation cancelled.\n";
    }
}

void viewClientReservations() {
    SkydiveSystem* system = SkydiveSystem::getInstance();
    auto client = dynamic_cast<Client*>(system->getCurrentUser().get());
    
    if (!client) {
        std::cout << "\nError: You must be logged in as a client to view your reservations.\n";
        return;
    }
    
    auto reservations = system->getClientReservations(client->getUserID());
    
    if (reservations.empty()) {
        std::cout << "\nYou have no reservations.\n";
        return;
    }
    
    std::cout << "\n===== Your Reservations =====\n";
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(12) << "Status" 
              << std::setw(20) << "Reservation Date" 
              << std::setw(15) << "Total Cost" 
              << "Loyalty Points\n";
    std::cout << std::string(70, '-') << "\n";
    
    for (const auto* res : reservations) {
        std::time_t reservationDate = res->getReservationDate();
        std::tm* resTime = std::localtime(&reservationDate);
        std::stringstream dateStr;
        dateStr << std::put_time(resTime, "%Y-%m-%d %H:%M");
        
        std::cout << std::left << std::setw(5) << res->getReservationID()
                  << std::setw(12) << res->getStatusAsString()
                  << std::setw(20) << dateStr.str()
                  << std::setw(15) << res->getTotalCost()
                  << res->getLoyaltyPointsEarned() << "\n";
    }
    
    // Option to cancel a reservation
    std::cout << "\nDo you want to cancel any reservation? (yes/no): ";
    std::string wantCancel;
    std::cin >> wantCancel;
    
    if (wantCancel == "yes") {
        int resID;
        std::cout << "Enter reservation ID to cancel: ";
        std::cin >> resID;
        
        if (system->cancelReservation(resID)) {
            std::cout << "Reservation successfully cancelled.\n";
        } else {
            std::cout << "Failed to cancel reservation. It may be invalid or already in progress.\n";
        }
    }
}

void handleAdminEquipment() {
    SkydiveSystem* system = SkydiveSystem::getInstance();
    auto admin = dynamic_cast<Administrator*>(system->getCurrentUser().get());
    
    if (!admin) {
        std::cout << "\nError: You must be logged in as an administrator to manage equipment.\n";
        return;
    }
    
    std::cout << "\n===== Equipment Management =====\n";
    std::cout << "1. Add New Equipment\n";
    std::cout << "2. Update Existing Equipment\n";
    std::cout << "3. View All Equipment\n";
    std::cout << "4. Back to Admin Menu\n";
    std::cout << "Please enter your choice: ";
    
    char choice;
    std::cin >> choice;
    
    switch (choice) {
        case '1': {
            // Add new equipment
            std::string typeStr, size;
            double price;
            int loyaltyPoints;
            
            std::cout << "\n===== Add New Equipment =====\n";
            std::cout << "Equipment Type (RIG, ALTIMETER, HELMET, JUMPSUIT): ";
            std::cin >> typeStr;
            std::cout << "Size (or model for altimeters): ";
            std::cin.ignore();
            std::getline(std::cin, size);
            std::cout << "Price per day: ";
            std::cin >> price;
            std::cout << "Loyalty points awarded: ";
            std::cin >> loyaltyPoints;
            
            EquipmentType type;
            if (typeStr == "RIG") {
                type = EquipmentType::RIG;
            } else if (typeStr == "ALTIMETER") {
                type = EquipmentType::ALTIMETER;
            } else if (typeStr == "HELMET") {
                type = EquipmentType::HELMET;
            } else if (typeStr == "JUMPSUIT") {
                type = EquipmentType::JUMPSUIT;
            } else {
                std::cout << "Invalid equipment type.\n";
                return;
            }
            
            if (system->addEquipment(type, size, price, loyaltyPoints)) {
                std::cout << "Equipment added successfully!\n";
            } else {
                std::cout << "Failed to add equipment.\n";
            }
            break;
        }
        case '2': {
            // Update existing equipment
            int equipID;
            std::string size;
            double price;
            std::string statusStr;
            
            // First, display all equipment
            auto allEquip = system->getAvailableEquipment(RIG, "");
            displayEquipment(allEquip);
            
            std::cout << "\n===== Update Equipment =====\n";
            std::cout << "Enter equipment ID to update: ";
            std::cin >> equipID;
            
            Equipment* equip = system->getEquipment(equipID);
            if (!equip) {
                std::cout << "Equipment ID not found.\n";
                return;
            }
            
            std::cout << "Current Size: " << equip->getSize() << "\n";
            std::cout << "New Size (press Enter to keep current): ";
            std::cin.ignore();
            std::getline(std::cin, size);
            if (size.empty()) {
                size = equip->getSize();
            }
            
            std::cout << "Current Price per Day: " << equip->getPricePerDay() << "\n";
            std::cout << "New Price per Day (enter 0 to keep current): ";
            std::cin >> price;
            if (price == 0) {
                price = equip->getPricePerDay();
            }
            
            std::cout << "Current Status: " << (equip->isAvailable() ? "Available" : "Reserved") << "\n";
            std::cout << "New Status (available/reserved): ";
            std::cin >> statusStr;
            bool available = (statusStr == "available");
            
            if (system->updateEquipment(equipID, size, price, available)) {
                std::cout << "Equipment updated successfully!\n";
            } else {
                std::cout << "Failed to update equipment.\n";
            }
            break;
        }
        case '3': {
            // View all equipment
            auto allEquip = system->getAvailableEquipment(RIG, "");
            displayEquipment(allEquip);
            break;
        }
        case '4':
            // Back to admin menu
            break;
        default:
            std::cout << "\nInvalid choice.\n";
            break;
    }
}

void handleAdminPlaneSlots() {
    SkydiveSystem* system = SkydiveSystem::getInstance();
    auto admin = dynamic_cast<Administrator*>(system->getCurrentUser().get());
    
    if (!admin) {
        std::cout << "\nError: You must be logged in as an administrator to manage plane slots.\n";
        return;
    }
    
    std::cout << "\n===== Plane Slot Management =====\n";
    std::cout << "1. Add New Plane Slot\n";
    std::cout << "2. Update Existing Plane Slot\n";
    std::cout << "3. View All Plane Slots\n";
    std::cout << "4. Back to Admin Menu\n";
    std::cout << "Please enter your choice: ";
    
    char choice;
    std::cin >> choice;
    
    switch (choice) {
        case '1': {
            // Add new plane slot
            std::string dateTimeStr;
            int capacity, altitude;
            double price;
            
            std::cout << "\n===== Add New Plane Slot =====\n";
            std::cout << "Departure Date (YYYY-MM-DD HH:MM): ";
            std::cin.ignore();
            std::getline(std::cin, dateTimeStr);
            
            std::tm timeInfo = {};
            std::istringstream ss(dateTimeStr);
            ss >> std::get_time(&timeInfo, "%Y-%m-%d %H:%M");
            std::time_t departureTime = std::mktime(&timeInfo);
            
            std::cout << "Capacity (number of seats): ";
            std::cin >> capacity;
            std::cout << "Price per seat: ";
            std::cin >> price;
            std::cout << "Altitude (in feet): ";
            std::cin >> altitude;
            
            if (system->addPlaneSlot(departureTime, capacity, price, altitude)) {
                std::cout << "Plane slot added successfully!\n";
            } else {
                std::cout << "Failed to add plane slot.\n";
            }
            break;
        }
        case '2': {
            // Update existing plane slot
            int slotID;
            std::string dateTimeStr;
            double price;
            std::string statusStr;
            
            // First, display all plane slots
            std::time_t now = std::time(nullptr);
            std::time_t oneYearLater = now + (365 * 24 * 60 * 60);
            auto allSlots = system->getAvailablePlaneSlots(now, oneYearLater);
            
            std::cout << "\n===== Available Plane Slots =====\n";
            displayPlaneSlots(allSlots);
            
            std::cout << "\n===== Update Plane Slot =====\n";
            std::cout << "Enter plane slot ID to update: ";
            std::cin >> slotID;
            
            PlaneSlot* slot = system->getPlaneSlot(slotID);
            if (!slot) {
                std::cout << "Plane slot ID not found.\n";
                return;
            }
            
            std::cout << "Current Departure Time: " << slot->getFormattedDepartureTime() << "\n";
            std::cout << "New Departure Time (YYYY-MM-DD HH:MM, press Enter to keep current): ";
            std::cin.ignore();
            std::getline(std::cin, dateTimeStr);
            
            std::time_t departureTime = slot->getDepartureTime();
            if (!dateTimeStr.empty()) {
                std::tm timeInfo = {};
                std::istringstream ss(dateTimeStr);
                ss >> std::get_time(&timeInfo, "%Y-%m-%d %H:%M");
                departureTime = std::mktime(&timeInfo);
            }
            
            std::cout << "Current Price per Seat: " << slot->getPricePerSeat() << "\n";
            std::cout << "New Price per Seat (enter 0 to keep current): ";
            std::cin >> price;
            if (price == 0) {
                price = slot->getPricePerSeat();
            }
            
            std::cout << "Current Status: " << (slot->isActive() ? "Active" : "Inactive") << "\n";
            std::cout << "New Status (active/inactive): ";
            std::cin >> statusStr;
            bool active = (statusStr == "active");
            
            if (system->updatePlaneSlot(slotID, departureTime, price, active)) {
                std::cout << "Plane slot updated successfully!\n";
            } else {
                std::cout << "Failed to update plane slot.\n";
            }
            break;
        }
        case '3': {
            // View all plane slots
            std::time_t now = std::time(nullptr);
            std::time_t oneYearLater = now + (365 * 24 * 60 * 60);
            auto allSlots = system->getAvailablePlaneSlots(now, oneYearLater);
            
            std::cout << "\n===== All Plane Slots =====\n";
            displayPlaneSlots(allSlots);
            break;
        }
        case '4':
            // Back to admin menu
            break;
        default:
            std::cout << "\nInvalid choice.\n";
            break;
    }
}
void viewAdminReservations() {
    SkydiveSystem* system = SkydiveSystem::getInstance();
    auto admin = dynamic_cast<Administrator*>(system->getCurrentUser().get());
    
    if (!admin) {
        std::cout << "\nError: You must be logged in as an administrator to view all reservations.\n";
        return;
    }
    
    // Get time range for reservations
    std::cout << "\n===== View Reservations =====\n";
    std::cout << "1. View Today's Reservations\n";
    std::cout << "2. View This Week's Reservations\n";
    std::cout << "3. View This Month's Reservations\n";
    std::cout << "4. View All Reservations\n";
    std::cout << "5. Back to Admin Menu\n";
    std::cout << "Please enter your choice: ";
    
    char choice;
    std::cin >> choice;
    
    std::time_t now = std::time(nullptr);
    std::time_t startDate, endDate;
    
    switch (choice) {
        case '1': {
            // Today's reservations
            std::tm* localTime = std::localtime(&now);
            localTime->tm_hour = 0;
            localTime->tm_min = 0;
            localTime->tm_sec = 0;
            startDate = std::mktime(localTime);
            
            localTime->tm_hour = 23;
            localTime->tm_min = 59;
            localTime->tm_sec = 59;
            endDate = std::mktime(localTime);
            break;
        }
        case '2': {
            // This week's reservations
            std::tm* localTime = std::localtime(&now);
            int currentDay = localTime->tm_wday; // 0 = Sunday, 6 = Saturday
            
            // Set to beginning of week (Sunday)
            localTime->tm_mday -= currentDay;
            localTime->tm_hour = 0;
            localTime->tm_min = 0;
            localTime->tm_sec = 0;
            startDate = std::mktime(localTime);
            
            // Set to end of week (Saturday)
            localTime->tm_mday += 6;
            localTime->tm_hour = 23;
            localTime->tm_min = 59;
            localTime->tm_sec = 59;
            endDate = std::mktime(localTime);
            break;
        }
        case '3': {
            // This month's reservations
            std::tm* localTime = std::localtime(&now);
            
            // Set to beginning of month
            localTime->tm_mday = 1;
            localTime->tm_hour = 0;
            localTime->tm_min = 0;
            localTime->tm_sec = 0;
            startDate = std::mktime(localTime);
            
            // Find last day of month
            // Find last day of month by going to next month and then back
            localTime->tm_mon++; // Move to next month
            if (localTime->tm_mon > 11) {
                localTime->tm_mon = 0;
                localTime->tm_year++;
            }
            localTime->tm_mday = 0; // Last day of previous month
            
            localTime->tm_hour = 23;
            localTime->tm_min = 59;
            localTime->tm_sec = 59;
            endDate = std::mktime(localTime);
            break;
        }
        case '4': {
            // All reservations - use a very wide range
            startDate = 0; // Beginning of time
            endDate = now + (10 * 365 * 24 * 60 * 60); // 10 years in the future
            break;
        }
        case '5':
            // Back to admin menu
            return;
        default:
            std::cout << "\nInvalid choice.\n";
            return;
    }
    
    // Get reservations in the selected period
    auto reservations = system->getReservationsInPeriod(startDate, endDate);
    
    if (reservations.empty()) {
        std::cout << "\nNo reservations found for the selected period.\n";
        return;
    }
    
    std::cout << "\n===== Reservations for Selected Period =====\n";
    std::cout << std::left << std::setw(5) << "ID" 
              << std::setw(10) << "Client ID" 
              << std::setw(12) << "Status" 
              << std::setw(20) << "Reservation Date" 
              << std::setw(15) << "Total Cost" 
              << "Loyalty Points\n";
    std::cout << std::string(80, '-') << "\n";
    
    for (const auto* res : reservations) {
        std::time_t reservationDate = res->getReservationDate();
        std::tm* resTime = std::localtime(&reservationDate);
        std::stringstream dateStr;
        dateStr << std::put_time(resTime, "%Y-%m-%d %H:%M");
        
        std::cout << std::left << std::setw(5) << res->getReservationID()
                  << std::setw(10) << res->getClientID()
                  << std::setw(12) << res->getStatusAsString()
                  << std::setw(20) << dateStr.str()
                  << std::setw(15) << res->getTotalCost()
                  << res->getLoyaltyPointsEarned() << "\n";
    }
    
    // Option to update reservation status
    std::cout << "\nDo you want to update any reservation status? (yes/no): ";
    std::string wantUpdate;
    std::cin >> wantUpdate;
    
    if (wantUpdate == "yes") {
        int resID;
        std::cout << "Enter reservation ID to update: ";
        std::cin >> resID;
        
        std::cout << "Select new status:\n";
        std::cout << "1. Confirmed\n";
        std::cout << "2. In Progress\n";
        std::cout << "3. Completed\n";
        std::cout << "4. Cancelled\n";
        std::cout << "Enter choice: ";
        
        char statusChoice;
        std::cin >> statusChoice;
        
        ReservationStatus newStatus;
        switch (statusChoice) {
            case '1':
                newStatus = ReservationStatus::CONFIRMED;
                break;
            case '2':
                newStatus = ReservationStatus::IN_PROGRESS;
                break;
            case '3':
                newStatus = ReservationStatus::COMPLETED;
                break;
            case '4':
                newStatus = ReservationStatus::CANCELLED;
                break;
            default:
                std::cout << "Invalid status choice.\n";
                return;
        }
        
        if (system->updateReservationStatus(resID, newStatus)) {
            std::cout << "Reservation status updated successfully.\n";
            
            // If status is completed, add loyalty points to client
            if (newStatus == ReservationStatus::COMPLETED) {
                auto res = system->getReservation(resID);
                if (res) {
                    auto client = dynamic_cast<Client*>(system->getCurrentUser().get());
                    if (client) {
                        client->addLoyaltyPoints(res->getLoyaltyPointsEarned());
                        std::cout << "Added " << res->getLoyaltyPointsEarned() 
                                  << " loyalty points to client account.\n";
                    }
                }
            }
        } else {
            std::cout << "Failed to update reservation status. Invalid status transition.\n";
        }
    }
}

void viewSystemStatistics() {
    SkydiveSystem* system = SkydiveSystem::getInstance();
    auto admin = dynamic_cast<Administrator*>(system->getCurrentUser().get());
    
    if (!admin) {
        std::cout << "\nError: You must be logged in as an administrator to view system statistics.\n";
        return;
    }
    
    std::cout << "\n===== System Statistics =====\n";
    std::cout << "1. Revenue Statistics\n";
    std::cout << "2. Equipment Usage Statistics\n";
    std::cout << "3. Back to Admin Menu\n";
    std::cout << "Please enter your choice: ";
    
    char choice;
    std::cin >> choice;
    
    switch (choice) {
        case '1': {
            // Revenue statistics
            std::cout << "\n===== Revenue Statistics =====\n";
            std::cout << "1. Today's Revenue\n";
            std::cout << "2. This Week's Revenue\n";
            std::cout << "3. This Month's Revenue\n";
            std::cout << "4. Total Revenue\n";
            std::cout << "Please enter your choice: ";
            
            char revenueChoice;
            std::cin >> revenueChoice;
            
            std::time_t now = std::time(nullptr);
            std::time_t startDate, endDate;
            
            switch (revenueChoice) {
                case '1': {
                    // Today's revenue
                    std::tm* localTime = std::localtime(&now);
                    localTime->tm_hour = 0;
                    localTime->tm_min = 0;
                    localTime->tm_sec = 0;
                    startDate = std::mktime(localTime);
                    
                    localTime->tm_hour = 23;
                    localTime->tm_min = 59;
                    localTime->tm_sec = 59;
                    endDate = std::mktime(localTime);
                    
                    double revenue = system->calculateTotalRevenue(startDate, endDate);
                    std::cout << "\nToday's Revenue: " << revenue << " lei\n";
                    break;
                }
                case '2': {
                    // This week's revenue
                    std::tm* localTime = std::localtime(&now);
                    int currentDay = localTime->tm_wday; // 0 = Sunday, 6 = Saturday
                    
                    // Set to beginning of week (Sunday)
                    localTime->tm_mday -= currentDay;
                    localTime->tm_hour = 0;
                    localTime->tm_min = 0;
                    localTime->tm_sec = 0;
                    startDate = std::mktime(localTime);
                    
                    // Set to end of week (Saturday)
                    localTime->tm_mday += 6;
                    localTime->tm_hour = 23;
                    localTime->tm_min = 59;
                    localTime->tm_sec = 59;
                    endDate = std::mktime(localTime);
                    
                    double revenue = system->calculateTotalRevenue(startDate, endDate);
                    std::cout << "\nThis Week's Revenue: " << revenue << " lei\n";
                    break;
                }
                case '3': {
                    // This month's revenue
                    std::tm* localTime = std::localtime(&now);
                    
                    // Set to beginning of month
                    localTime->tm_mday = 1;
                    localTime->tm_hour = 0;
                    localTime->tm_min = 0;
                    localTime->tm_sec = 0;
                    startDate = std::mktime(localTime);
                    
                    // Find last day of month
                    // Find last day of month by going to next month and then back
                    localTime->tm_mon++; // Move to next month
                    if (localTime->tm_mon > 11) {
                        localTime->tm_mon = 0;
                        localTime->tm_year++;
                    }
                    localTime->tm_mday = 0; // Last day of previous month
                    
                    localTime->tm_hour = 23;
                    localTime->tm_min = 59;
                    localTime->tm_sec = 59;
                    endDate = std::mktime(localTime);
                    
                    double revenue = system->calculateTotalRevenue(startDate, endDate);
                    std::cout << "\nThis Month's Revenue: " << revenue << " lei\n";
                    break;
                }
                case '4': {
                    // Total revenue - use a very wide range
                    startDate = 0; // Beginning of time
                    endDate = now + (10 * 365 * 24 * 60 * 60); // 10 years in the future
                    
                    double revenue = system->calculateTotalRevenue(startDate, endDate);
                    std::cout << "\nTotal Revenue: " << revenue << " lei\n";
                    break;
                }
                default:
                    std::cout << "\nInvalid choice.\n";
                    break;
            }
            break;
        }
        case '2': {
            // Equipment usage statistics
            auto usageStats = system->getEquipmentUsageStats();
            
            std::cout << "\n===== Equipment Usage Statistics =====\n";
            std::cout << "RIG: " << usageStats[EquipmentType::RIG] << " in use\n";
            std::cout << "ALTIMETER: " << usageStats[EquipmentType::ALTIMETER] << " in use\n";
            std::cout << "HELMET: " << usageStats[EquipmentType::HELMET] << " in use\n";
            std::cout << "JUMPSUIT: " << usageStats[EquipmentType::JUMPSUIT] << " in use\n";
            break;
        }
        case '3':
            // Back to admin menu
            break;
        default:
            std::cout << "\nInvalid choice.\n";
            break;
    }
}