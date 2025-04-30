#pragma once
#include <string>

using namespace std;
struct UserInfo {
    string nama;
    string password;
    string role;

    UserInfo() = default;
    UserInfo(string& nama, string& password, string& role)
        : nama(nama), password(password), role(role) {}
};