#pragma once
#include <string>
#include <unordered_map>
#include "UserInfo.h"

class UserDB {
private:
    std::unordered_map<std::string, UserInfo> userMap;
    std::string dbFilePath;

    // Helper functions
    std::string getPasswordInput(const std::string& prompt) const;
    bool validateRole(const std::string& role) const;
    bool validatePassword(const std::string& password) const;

public:
    UserDB(std::string filelocation);
    
    // UI Functions
    void showLoginUI() const;
    void showRegistrationUI();
    void showResetPasswordUI();
    
    bool login();
    
    // Core Functions
    bool addUser(const std::string& username, const std::string& nama, 
                const std::string& password, const std::string& role);
    bool saveToFile();
    bool resetPassword(const std::string& username, const std::string& nama, 
                      const std::string& newPassword);
    bool checkLogin(const std::string& username, const std::string& password) const;
    
};