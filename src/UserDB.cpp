#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "UserDB.h"

using namespace std;

UserDB::UserDB(std::string filelocation)
    : dbFilePath((std::filesystem::path("..") / filelocation).string()) {

    ifstream file(dbFilePath);

    if (!file) {
        // Buat file baru dengan header jika tidak ada
        ofstream createFile(dbFilePath);
        if (createFile) {
            createFile << "username,nama,password,role\n";
            cout << "File baru dibuat: " << filesystem::absolute(dbFilePath) << endl;
        } else {
            cerr << "Gagal membuat file di: " << filesystem::absolute(dbFilePath) << endl;
        }
        return;
    }

    // Baca file yang sudah ada
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string username, nama, pass, role;

        getline(ss, username, ',');
        getline(ss, nama, ',');
        getline(ss, pass, ',');
        getline(ss, role, ',');

        userMap[username] = UserInfo(nama, pass, role);
    }
}

bool UserDB::addUser(const string& username, const string& nama, const string& password, const string& role) {
    if (userMap.count(username)) {
        cout << "Username sudah digunakan\n";
        return false; // Username sudah ada
    }

    userMap[username] = UserInfo(nama, password, role);

    if (!saveToFile()) return false;

    return true;
}

bool UserDB::saveToFile() {
    std::string tempPath = dbFilePath + ".tmp";
    ofstream file(tempPath);
    if (!file) {
        cerr << "Gagal menyimpan sementara ke: " << tempPath << endl;
        return false;
    }

    file << "username,nama,password,role\n";
    for (const auto& [username, userInfo] : userMap) {
        file << username << ","
             << userInfo.nama << ","
             << userInfo.password << ","
             << userInfo.role << "\n";
    }

    file.close();
    std::filesystem::rename(tempPath, dbFilePath);  // Atomically replaces original
    cout << "Data disimpan ke: " << filesystem::absolute(dbFilePath) << endl;
    return true;
}

bool UserDB::resetPassword(const string& username, const string& nama, const string& newPassword) {
    auto it = userMap.find(username);
    if (it == userMap.end() || it->second.nama != nama) {
        cout << "Data tidak cocok" << endl;
        return false;
    }
    it->second.password = newPassword;

    if (!saveToFile()) return false;
    return true;
}

bool UserDB::checkLogin(const std::string& username, const std::string& password) {
    // Check apakah user ada
    auto it = userMap.find(username);
    if (it == userMap.end()) return false;

    // Check apakah password cocok
    const UserInfo& user = it->second;
    if (user.password != password) return false;

    return true;
}