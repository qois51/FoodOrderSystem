#pragma once
#include <string>

using namespace std;
struct UserInfo {
    string nama;
    string password;
    string role;

    UserInfo() = default;
    UserInfo(const string& nama, const string& password, const string& role)
    : nama(nama), password(password), role(role) {}

};