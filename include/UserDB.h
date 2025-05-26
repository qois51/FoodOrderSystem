#pragma once
#include <string>
#include <unordered_map>
#include "UserInfo.h"
#include "OrdersDB.h"

class UserDB {
private:
    std::string currentUser;
    std::unordered_map<std::string, UserInfo> userMap;
    std::string dbFilePath;
    OrdersDB* ordersDB;

    std::string getPasswordInput(const std::string& prompt) const;
    bool validateRole(const std::string& role) const;
    bool validatePassword(const std::string& password) const;

public:
    UserDB(std::string filelocation, OrdersDB* ordersDB);
    
    void showLoginUI() const;
    void showRegistrationUI();
    void showResetPasswordUI();
    
    std::string login();
    
    bool addUser(const std::string& username, const std::string& nama, 
                const std::string& password, const std::string& role);
    bool saveToFile();
    bool resetPassword(const std::string& username, const std::string& nama, 
                      const std::string& newPassword);
    bool checkLogin(const std::string& username, const std::string& password) const;
    
    std::string getUserRole(const std::string& username) const;
    void displayUserActivities(const std::string& username) const;

    void viewOrderHistoryCustomer(const std::string& username) const;
    void viewAllOrdersForStaff() const;
};