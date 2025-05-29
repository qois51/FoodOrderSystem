#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>

#include "UserDB.h"
#include "OrdersDB.h"
#include "Clear.h"
#include "headers.h"
#include "MenuItem.h"

using namespace std;

UserDB::UserDB(string filelocation, OrdersDB* ordersDBPtr)
    : dbFilePath(filelocation),
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

void UserDB::showRegistrationUI() {
    clearConsole();
    showMainHeader();

    string username, nama, password, role;

    cout << "=== REGISTRASI ===" << endl;
    cout << "Masukkan Username: ";
    getline(cin, username);
    cout << "Masukkan Nama Lengkap: ";
    getline(cin, nama);

    while (true) {
        password = getPasswordInput("Masukkan Password (min 6 karakter): ");
        if (validatePassword(password)) break;
        cout << "\nPassword terlalu pendek!\n";
    }

    while (true) {
        cout << "Role (pelanggan/petugas): ";
        getline(cin, role);
        if (validateRole(role)) break;
        cout << "\nRole harus 'pelanggan' atau 'petugas'\n";
    }

    if (addUser(username, nama, password, role)) {
        cout << "\nAkun berhasil dibuat!\n";
        pauseScreen();
    }
}

void UserDB::showResetPasswordUI() {
    clearConsole();
    showMainHeader();

    string username, nama, newPass;

    cout << "=== LUPA PASSWORD ===" << endl;
    cout << "Username: ";
    getline(cin, username);
    cout << "Nama Lengkap (autentikasi): ";
    getline(cin, nama);

    while (true) {
        newPass = getPasswordInput("Password Baru (min 6 karakter): ");
        if (validatePassword(newPass)) break;
        cout << "Password terlalu pendek!\n";
        delay(2);
    }

    if (resetPassword(username, nama, newPass)) {
        cout << "\nPassword berhasil diubah\n";
        delay(2);
    }
}

bool UserDB::addUser(const string& username, const string& nama,
                     const string& password, const string& role) {
    if (userMap.count(username)) {
        cout << "\nUsername sudah digunakan\n";
        delay(2);
        return false;
    }
    userMap[username] = UserInfo(nama, password, role);
    return saveToFile();
}

bool UserDB::saveToFile() {
    string tempPath = dbFilePath + ".tmp";
    ofstream file(tempPath);
    if (!file) {
        cerr << "\nGagal menyimpan sementara ke: " << tempPath << endl;
        delay(2);
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
    return true;
}

bool UserDB::resetPassword(const string& username, const string& nama,
                           const string& newPassword) {
    auto it = userMap.find(username);
    if (it == userMap.end() || it->second.nama != nama) {
        cout << "\nData tidak cocok" << endl;
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
    clearConsole();    
    showMainHeader();

    std::string username;
    std::string password;

    std::cout << "=== LOGIN ===" << std::endl;
    std::cout << "Silahkan masukkan username: ";
    std::getline(std::cin, username);

    password = getPasswordInput("Silahkan masukkan password: ");

    if (checkLogin(username, password)) {
        currentUser = username;
        auto it = userMap.find(username);
        if (it != userMap.end()) {
            currentUserFullName = it->second.nama;
        };

        return username;
    } else {
        std::cout << "\nUsername atau password salah\n";
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
            clearConsole();
            showMainHeader();

            std::cout << "================================\n";
            std::cout << "  SELAMAT DATANG, " << currentUserFullName<< "!\n";
            std::cout << "  MENU AKTIVITAS PELANGGAN\n";
            std::cout << "================================\n";
            std::cout << " [1] Lihat Pesanan\n";
            std::cout << " [2] Buat Pesanan Baru\n";
            std::cout << " [3] Logout\n";
            std::cout << "-------------------------------------\n";

            int choice;
            bool inputValid = false;
            
            while (!inputValid) {
                std::cout << "Pilih aktivitas (1/2/3): ";
                
                if (!(std::cin >> choice)) {
                    std::cout << "\nInput tidak valid. Masukkan angka!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    pauseScreen();
                    clearConsole();
                    showMainHeader();

                    std::cout << "================================\n";
                    std::cout << "  SELAMAT DATANG, " << currentUserFullName<< "!\n";
                    std::cout << "  MENU AKTIVITAS PELANGGAN\n";
                    std::cout << "================================\n";
                    std::cout << " [1] Lihat Pesanan\n";
                    std::cout << " [2] Buat Pesanan Baru\n";
                    std::cout << " [3] Logout\n";
                    std::cout << "-------------------------------------\n";
                    continue;
                }
                
                std::cin.ignore();
                inputValid = true;
            }

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
                        pauseScreen();
                    }
                    break;
                default:
                    std::cout << "\nPilihan tidak valid.\n";
                    pauseScreen();
                    break;
            }
        }
    } else if(role == "petugas") {
        while (true) {
            clearConsole();
            showMainHeader();

            std::cout << "================================\n";
            std::cout << "  SELAMAT DATANG, " << currentUserFullName<< "!\n";
            std::cout << "  MENU AKTIVITAS PETUGAS\n";
            std::cout << "================================\n";
            std::cout << " [1] Kelola Pesanan\n";
            std::cout << " [2] Lihat Semua Pesanan\n";
            std::cout << " [3] Logout\n";
            std::cout << "-------------------------------------\n";

            int choice;
            bool inputValid = false;
            
            while (!inputValid) {
                std::cout << "Pilih aktivitas (1/2/3): ";
                
                if (!(std::cin >> choice)) {
                    std::cout << "\nInput tidak valid. Masukkan angka!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    pauseScreen();
                    
                    clearConsole();
                    showMainHeader();
                    std::cout << "================================\n";
                    std::cout << "  SELAMAT DATANG, " << currentUserFullName<< "!\n";
                    std::cout << "  MENU AKTIVITAS PETUGAS\n";
                    std::cout << "================================\n";
                    std::cout << " [1] Kelola Pesanan\n";
                    std::cout << " [2] Lihat Semua Pesanan\n";
                    std::cout << " [3] Logout\n";
                    std::cout << "-------------------------------------\n";
                    continue;
                }
                
                std::cin.ignore();
                inputValid = true;
            }

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
                        pauseScreen();
                    }
                    break;
                case 2:
                    viewAllOrdersForStaff();
                    break;
                default:
                    std::cout << "\nPilihan tidak valid.\n";
                    pauseScreen();
                    break;
            }
        }
    } else {
        std::cout << "Role tidak dikenal atau tidak ditemukan.\n";
        pauseScreen();
    }
}

void UserDB::viewOrderHistoryCustomer(const std::string& username) const {
    clearConsole();
    showMainHeader();

    if (!ordersDB) {
        std::cout << "Sistem pesanan tidak tersedia.\n";
        return;
    }

    std::cout << "===== RIWAYAT PESANAN SAYA (" << username << ") =====" << std::endl;

    std::vector<std::string> customerOrderIds = ordersDB->getSortedOrderIds("", username);

    if (customerOrderIds.empty()) {
        std::cout << "Anda belum memiliki pesanan.\n";
        std::cout << "Tekan Enter untuk kembali...";
        std::cin.ignore();
        return;
    }

    // Sort
    const unordered_map<string, OrderInfo>& allOrders = ordersDB->getOrderList();

    // Price data
    extern std::unordered_map<std::string, std::vector<MenuItem>> menuItems;
    
    for (const std::string& orderId : customerOrderIds) {
        auto it = allOrders.find(orderId);
        if (it != allOrders.end()) {
            const OrderInfo& order = it->second;

            std::cout << "----------------------------------------\n";
            std::cout << "ID Pesanan    : " << orderId << std::endl;
            std::cout << "Tanggal       : " << order.tanggalPemesanan.year << "-"
                      << std::setw(2) << std::setfill('0') << order.tanggalPemesanan.month << "-"
                      << std::setw(2) << std::setfill('0') << order.tanggalPemesanan.day << std::endl;
            std::cout << "Status        : " << order.status << std::endl;
            std::cout << "Item Pesanan  :\n";

            int totalPrice = 0;
            for (const auto& pair : order.itemPesanan) {
                const std::string& itemName = pair.first;
                int quantity = pair.second;
                
                int itemPrice = 0;
                for (const auto& [category, items] : menuItems) {
                    for (const auto& item : items) {
                        if (item.name == itemName) {
                            itemPrice = item.price;
                            break;
                        }
                    }
                    if (itemPrice > 0) break;
                }
                
                int subtotal = itemPrice * quantity;
                totalPrice += subtotal;
                
                std::cout << "  - " << itemName 
                          << " (Rp" << itemPrice << " x " << quantity 
                          << ") = Rp" << subtotal << "\n";
            }
            
            std::cout << "\nTOTAL HARGA: Rp" << totalPrice << "\n";
            std::cout << "----------------------------------------\n";
        }
    }
    
    std::cout << "Tekan Enter untuk kembali...";
    std::cin.ignore();
    clearConsole();
}

void UserDB::viewAllOrdersForStaff() const {
    clearConsole();
    showMainHeader();

    if (!ordersDB) {
        std::cout << "Sistem pesanan tidak tersedia.\n";
        return;
    }

    std::cout << "===== DAFTAR SEMUA PESANAN =====" << std::endl;

    // Sort
    std::vector<std::string> sortedOrderIds = ordersDB->getSortedOrderIds();

    if (sortedOrderIds.empty()) {
        std::cout << "Belum ada pesanan yang masuk.\n";
        std::cout << "Tekan Enter untuk kembali...";
        std::cin.ignore();
        return;
    }

    // Order data
    const auto& allOrders = ordersDB->getOrderList();

    // Price data
    extern std::unordered_map<std::string, std::vector<MenuItem>> menuItems;

    for (const auto& orderId : sortedOrderIds) {
        const auto& order = allOrders.at(orderId);
        
        std::cout << "----------------------------------------\n";
        std::cout << "ID Pesanan    : " << orderId << std::endl;
        std::cout << "Pelanggan     : " << order.Pelanggan << std::endl;
        std::cout << "Tanggal       : " << order.tanggalPemesanan.year << "-"
                  << std::setw(2) << std::setfill('0') << order.tanggalPemesanan.month << "-"
                  << std::setw(2) << std::setfill('0') << order.tanggalPemesanan.day << std::endl;
        std::cout << "Status        : " << order.status << std::endl;
        std::cout << "Item Pesanan  :\n";

        int totalPrice = 0;
        for (const auto& pair : order.itemPesanan) {
            const std::string& itemName = pair.first;
            int quantity = pair.second;
            
            int itemPrice = 0;
            for (const auto& [category, items] : menuItems) {
                for (const auto& item : items) {
                    if (item.name == itemName) {
                        itemPrice = item.price;
                        break;
                    }
                }
                if (itemPrice > 0) break;
            }
            
            int subtotal = itemPrice * quantity;
            totalPrice += subtotal;
            
            std::cout << "  - " << itemName 
                      << " (Rp" << itemPrice << " x" << quantity 
                      << ") = Rp" << subtotal << "\n";
        }
        
        std::cout << "\nTOTAL HARGA: Rp" << totalPrice << "\n";
        std::cout << "----------------------------------------\n";
    }
    std::cout << "Tekan Enter untuk kembali...";
    std::cin.ignore();
}