#pragma once
#include <string>
#include <vector>

using namespace std;
struct UserInfo {
    string nama;
    string password;
    string role;

    vector<string> orderHistory;

    UserInfo() = default;
    UserInfo(const string& nama, const string& password, 
             const string& role, vector<string> orderHistory = {})
        : nama(nama), password(password), role(role), orderHistory(orderHistory) {}
};