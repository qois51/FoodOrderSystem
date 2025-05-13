#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "UserDB.h"

using namespace std;

UserDB::UserDB(string filelocation) : dbFilePath((filesystem::path("..") / filelocation).string()) {
    ifstream file(dbFilePath);

    if (!file) {
        filesystem::create_directories(filesystem::path(dbFilePath).parent_path());
        ofstream createFile(dbFilePath);
        if (createFile) {
            createFile << "username,nama,password,role\n";
            cout << "Created new file: " << filesystem::absolute(dbFilePath) << endl;
        } else {
            cerr << "Failed to create file at: " << filesystem::absolute(dbFilePath) << endl;
        }
        return;
    }

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
    cout << "Loaded data from: " << filesystem::absolute(dbFilePath) << endl;
}

string UserDB::getPasswordInput(const string& prompt) const {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

bool UserDB::validateRole(const string& role) const {
    return (role == "pelanggan" || role == "petugas");
}

bool UserDB::validatePassword(const string& password) const {
    return password.length() >= 6;
}

void UserDB::showLoginUI() const {
    string username, password;
    
    cout << "\n=== LOGIN ===" << endl;
    cout << "Username: ";
    getline(cin, username);
    password = getPasswordInput("Password: ");

    if (checkLogin(username, password)) {
        cout << "Login berhasil!\n";
    } else {
        cout << "Username atau password salah\n";
    }
}

void UserDB::showRegistrationUI() {
    string username, nama, password, role;

    cout << "\n=== REGISTRASI ===" << endl;
    cout << "Username: ";
    getline(cin, username);
    cout << "Nama Lengkap: ";
    getline(cin, nama);
    
    while (true) {
        password = getPasswordInput("Password (min 6 karakter): ");
        if (validatePassword(password)) break;
        cout << "Password terlalu pendek!\n";
    }

    while (true) {
        cout << "Role (pelanggan/petugas): ";
        getline(cin, role);
        if (validateRole(role)) break;
        cout << "Role harus 'pelanggan' atau 'petugas'\n";
    }

    if (addUser(username, nama, password, role)) {
        cout << "Akun berhasil dibuat!\n";
    }
}

void UserDB::showResetPasswordUI() {
    string username, nama, newPass;

    cout << "\n=== LUPA PASSWORD ===" << endl;
    cout << "Username: ";
    getline(cin, username);
    cout << "Nama Lengkap (autentikasi): ";
    getline(cin, nama);
    
    while (true) {
        newPass = getPasswordInput("Password Baru (min 6 karakter): ");
        if (validatePassword(newPass)) break;
        cout << "Password terlalu pendek!\n";
    }

    if (resetPassword(username, nama, newPass)) {
        cout << "Password berhasil diubah\n";
    }
}

bool UserDB::addUser(const string& username, const string& nama, 
                    const string& password, const string& role) {
    if (userMap.count(username)) {
        cout << "Username sudah digunakan\n";
        return false;
    }
    userMap[username] = UserInfo(nama, password, role);
    return saveToFile();
}

bool UserDB::saveToFile() {
    string tempPath = dbFilePath + ".tmp";
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
    filesystem::rename(tempPath, dbFilePath);
    cout << "Data disimpan ke: " << filesystem::absolute(dbFilePath) << endl;
    return true;
}

bool UserDB::resetPassword(const string& username, const string& nama, 
                          const string& newPassword) {
    auto it = userMap.find(username);
    if (it == userMap.end() || it->second.nama != nama) {
        cout << "Data tidak cocok" << endl;
        return false;
    }
    it->second.password = newPassword;
    return saveToFile();
}

bool UserDB::checkLogin(const string& username, const string& password) const {
    auto it = userMap.find(username);
    return it != userMap.end() && it->second.password == password;
}

bool UserDB::login() {
    std::string username;
    std::string password;

    std::cout << "\n=== LOGIN ===" << std::endl;
    std::cout << "Silahkan masukkan username: ";
    std::getline(std::cin, username);

    // Memanfaatkan fungsi getPasswordInput yang sudah ada untuk input password
    password = getPasswordInput("Silahkan masukkan password: ");

    if (checkLogin(username, password)) {
        std::cout << "Login berhasil!\n";
        return true;
    } else {
        std::cout << "Username atau password salah\n";
        return false;
    }
}