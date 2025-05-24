// --- FILE: UserDB.cpp (LENGKAP, Komentar Minimal) ---

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>

#include "UserDB.h"
#include "OrdersDB.h"

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
            cout << "Membuat file baru: " << filesystem::absolute(dbFilePath) << endl;
        } else {
            cerr << "Gagal membuat file di: " << filesystem::absolute(dbFilePath) << endl;
        }
        return;
    }

    string line;
    getline(file, line);

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
    cout << "Data dimuat dari: " << filesystem::absolute(dbFilePath) << endl;

    if (ordersDB) {
        for (const auto& [orderId, orderInfo] : ordersDB->getOrderList()) {
            const string& pelanggan = orderInfo.Pelanggan;

            auto it = userMap.find(pelanggan);
            if (it != userMap.end()) {
                it->second.orderHistory.push_back(orderId);
            }
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
    return "";
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
                    if (ordersDB) {
                        ordersDB->createNewOrder(username);
                    } else {
                        std::cout << "Sistem pesanan tidak tersedia.\n";
                    }
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
                    cout << "Proses Pesanan:\n";
                    if (ordersDB) {
                        ordersDB->processOrder();
                    } else {
                        std::cout << "Sistem pesanan tidak tersedia.\n";
                    }
                    break;
                case 2:
                    cout << "Riwayat Pesanan:\n";
                    std::cout << "Fitur ini belum diimplementasikan untuk petugas.\n";
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
    if (!ordersDB) {
        std::cout << "Sistem pesanan tidak tersedia.\n";
        return;
    }

    std::cout << "\n===== RIWAYAT PESANAN SAYA (" << username << ") =====" << std::endl;

    std::vector<std::string> customerOrderIds = ordersDB->getSortedOrderIds("", username);

    if (customerOrderIds.empty()) {
        std::cout << "Anda belum memiliki pesanan.\n";
        std::cout << "Tekan Enter untuk kembali...";
        std::cin.ignore();
        return;
    }

    const unordered_map<string, OrderInfo>& allOrders = ordersDB->getOrderList();

    for (const std::string& orderId : customerOrderIds) {
        auto it = allOrders.find(orderId);
        if (it != allOrders.end()) {
            const OrderInfo& order = it->second;

            std::cout << "----------------------------------------\n";
            std::cout << "ID Pesanan    : " << orderId << std::endl;
            // Format tanggal agar selalu 2 digit untuk bulan dan hari
            std::cout << "Tanggal       : " << order.tanggalPemesanan.year << "-"
                      << std::setw(2) << std::setfill('0') << order.tanggalPemesanan.month << "-"
                      << std::setw(2) << std::setfill('0') << order.tanggalPemesanan.day << std::endl;
            std::cout << "Status        : " << order.status << std::endl;
            std::cout << "Item Pesanan  :\n";

            for (const auto& pair : order.itemPesanan) {
                const std::string& itemName = pair.first;
                int quantity = pair.second;
                std::cout << "  - " << itemName << " (x" << quantity << ")\n";
            }
            std::cout << "----------------------------------------\n";
        }
    }
    std::cout << "Tekan Enter untuk kembali...";
    std::cin.ignore();
}
