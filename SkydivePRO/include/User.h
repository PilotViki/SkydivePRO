#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    int userID;
    std::string name;
    std::string email;
    std::string password;
    static int nextUserID;

public:
    // Constructors
    User();
    User(const std::string& name, const std::string& email, const std::string& password);
    
    // Virtual destructor for proper inheritance
    virtual ~User();
    
    // Getters
    int getUserID() const;
    std::string getName() const;
    std::string getEmail() const;
    
    // Setters
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPassword(const std::string& password);
    
    // Authentication methods
    bool authenticate(const std::string& email, const std::string& password) const;
    
    // Pure virtual method to define user role (makes User an abstract class)
    virtual std::string getRole() const = 0;
};

#endif // USER_H
