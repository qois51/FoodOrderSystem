#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "UserDB.h"

using namespace std;

UserDB::UserDB(string filelocation, OrdersDB* ordersDBPtr) 
    : dbFilePath((filesystem::path("..") / filelocation).string()),
      ordersDB(ordersDBPtr) {

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
    
    file.close();
    cout << "Loaded data from: " << filesystem::absolute(dbFilePath) << endl;

    cout << "Memasukkan History Pesanan ke Pelanggan" << endl;
    for (const auto& [orderId, orderInfo] : ordersDBPtr->getOrderList()) {
        const string& pelanggan = orderInfo.Pelanggan;

        auto it = userMap.find(pelanggan);
        if (it != userMap.end()) {
            it->second.orderHistory.push_back(orderId);
        }
    }
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

std::string UserDB::login() {
    std::string username;
    std::string password;

    std::cout << "\n=== LOGIN ===" << std::endl;
    std::cout << "Silahkan masukkan username: ";
    std::getline(std::cin, username);

    password = getPasswordInput("Silahkan masukkan password: ");

    if (checkLogin(username, password)) {
        std::cout << "Login berhasil!\n";
        currentUser = username;
        return username;
    } else {
        std::cout << "Username atau password salah\n";
        currentUser.clear();
        return "";
    }
}

std::string UserDB::getUserRole(const std::string& username) const {
    auto it = userMap.find(username);
    if (it != userMap.end()) {
        return it->second.role;
    }
    return ""; //kalau gak ketemu usernya
}

void UserDB::displayUserActivities(const std::string& username) const {
    std::string role = getUserRole(username);

    if(role == "pelanggan") {
        while(true) {
            std::cout << "\nAktivitas untuk Pelanggan:\n";
            std::cout << "1. Lihat Pesanan\n";
            std::cout << "2. Buat Pesanan Baru\n";
            std::cout << "3. Logout\n";
            cout << "Pilih aktivitas (1/2/3): ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 3) {
                cout << "Logout berhasil.\n";
                break;
            }

            switch(choice) {
                case 1:
                    UserDB::viewOrderHistoryCustomer(username);
                    break;
                case 2:
                    ordersDB->createNewOrder(username);
                    break;
                default:
                    std::cout << "Pilihan tidak valid.\n";
            }
        }
    } else if(role == "petugas") {
        while (true) {
            std::cout << "\nAktivitas untuk Petugas:\n";
            std::cout << "1. Kelola Pesanan\n";
            std::cout << "2. Lihat Riwayat Pesanan\n";
            std::cout << "3. Logout\n";
            cout << "Pilih aktivitas (1/2/3): ";

            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 3) {
                cout << "Logout berhasil.\n";
                break;
            }

            switch(choice) {
                case 1:
                    cout << "Process Orderan:\n";
                    ordersDB->processOrder();
                    break;
                case 2:
                    cout << "Riwayat Pesanan:\n";
                    break;
                default:
                    std::cout << "Pilihan tidak valid.\n";
            }
        }
    } else {
        std::cout << "Role tidak dikenal atau tidak ditemukan.\n";
    }
}

void UserDB::viewOrderHistoryCustomer(const std::string& username) const {
    auto it = userMap.find(username);
    if (it != userMap.end()) {
        cout << "Riwayat Pesanan untuk " << username << ":\n";
        bool found = false;
        for (const auto& [orderId, orderInfo] : ordersDB->getOrderList()) {
            if (orderInfo.Pelanggan == username) {
                found = true;
                cout << "----------------------------------------\n";
                cout << "ID Pesanan : " << orderId << "\n";
                cout << "Tanggal    : " << orderInfo.tanggalPemesanan.year << "-"
                     << orderInfo.tanggalPemesanan.month << "-"
                     << orderInfo.tanggalPemesanan.day << "\n";
                cout << "Status     : " << orderInfo.status << "\n";
                cout << "Item       :\n";
                for (const auto& [item, qty] : orderInfo.itemPesanan) {
                    cout << "  - " << item << " x" << qty << "\n";
                }
                cout << "----------------------------------------\n";
            }
        }
        if (!found) {
            cout << "Tidak ada pesanan untuk user ini.\n";
        }
    } else {
        cout << "User tidak ditemukan.\n";
    }
}