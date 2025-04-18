#include "../include/User.h"

// Initialize static member
int User::nextUserID = 1;

User::User() : userID(0), name(""), email(""), password("") {
}

User::User(const std::string& name, const std::string& email, const std::string& password)
    : userID(nextUserID++), name(name), email(email), password(password) {
}

User::~User() {
    // Virtual destructor implementation
}

int User::getUserID() const {
    return userID;
}

std::string User::getName() const {
    return name;
}

std::string User::getEmail() const {
    return email;
}

void User::setName(const std::string& name) {
    this->name = name;
}

void User::setEmail(const std::string& email) {
    this->email = email;
}

void User::setPassword(const std::string& password) {
    this->password = password;
}

bool User::authenticate(const std::string& email, const std::string& password) const {
    return this->email == email && this->password == password;
}
