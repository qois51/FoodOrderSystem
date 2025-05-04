#pragma once
#include <string>
#include <unordered_map>
#include "UserInfo.h"

using namespace std;

class UserDB {
private:
    unordered_map<string, UserInfo> userMap;
    string dbFilePath; // Menyimpan path file

public:
    UserDB(string filelocation);
    bool addUser(const string& username, const string& nama, const string& password, const string& role);
    void saveToFile(); // Tidak perlu parameter lagi
    bool resetPassword(const string& username, const string& nama, const string& newPassword);
};