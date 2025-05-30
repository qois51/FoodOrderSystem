#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iomanip>
#include <limits>

#include "UserDB.h"
#include "OrdersDB.h"
#include "Clear.h"
#include "headers.h"
#include "MenuItem.h"
#include "Stack.h"
#include "OrderAction.h"

using namespace std;

// Menyimpan riwayat order tiap user untuk fitur undo
static std::unordered_map<std::string, CustomStack<std::string>> userOrderHistory;

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
            std::cout << " [3] Batalkan Pesanan Terakhir (Undo)\n";
            std::cout << " [4] Logout\n";
            std::cout << "-------------------------------------\n";

            int choice;
            bool inputValid = false;
            
            while (!inputValid) {
                std::cout << "Pilih aktivitas (1/2/3/4): ";
                
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
                    std::cout << " [3] Batalkan Pesanan Terakhir (Undo)\n";
                    std::cout << " [4] Logout\n";
                    std::cout << "-------------------------------------\n";
                    continue;
                }
                
                std::cin.ignore();
                inputValid = true;
            }

            if (choice == 4) {
                cout << "Logout berhasil.\n";
                break;
            }

            switch(choice) {
                case 1:
                    UserDB::viewOrderHistoryCustomer(username);
                    break;
                case 2:
                    if (ordersDB) {
                        size_t orderCountBefore = ordersDB->getOrderList().size();
                        
                        ordersDB->createNewOrder(username);
                        
                        size_t orderCountAfter = ordersDB->getOrderList().size();
                        
                        // Jika order berhasil dibuat, simpan di stack untuk undo
                        if (orderCountAfter > orderCountBefore) {
                            std::string newOrderId = getLastOrderIdForUser(username);
                            
                            if (!newOrderId.empty()) {
                                userOrderHistory[username].push(newOrderId);
                                std::cout << "\nPesanan berhasil dibuat! ID: " << newOrderId << "\n";
                                std::cout << "Anda dapat membatalkan pesanan ini menggunakan opsi Undo.\n";
                            } else {
                                std::cout << "\nPesanan berhasil dibuat!\n";
                            }
                        } else {
                            std::cout << "\nPembuatan pesanan dibatalkan atau gagal.\n";
                        }
                        pauseScreen();
                    } else {
                        std::cout << "Sistem pesanan tidak tersedia.\n";
                        pauseScreen();
                    }
                    break;
                case 3:
                    undoLastOrder(username);
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

// Cari order paling terbaru untuk user ini
std::string UserDB::getLastOrderIdForUser(const std::string& username) const {
    if (!ordersDB) return "";
    
    const auto& allOrders = ordersDB->getOrderList();
    std::string mostRecentOrderId = "";
    
    // Asumsi order ID berurutan chronological
    for (const auto& [orderId, orderInfo] : allOrders) {
        if (orderInfo.Pelanggan == username) {
            if (mostRecentOrderId.empty() || orderId > mostRecentOrderId) {
                mostRecentOrderId = orderId;
            }
        }
    }
    
    return mostRecentOrderId;
}

// Alternative cari berdasarkan tanggal
std::string UserDB::getLastOrderIdForUserByDate(const std::string& username) const {
    if (!ordersDB) return "";
    
    const auto& allOrders = ordersDB->getOrderList();
    std::string mostRecentOrderId = "";
    Date mostRecentDate = {0, 0, 0};
    
    for (const auto& [orderId, orderInfo] : allOrders) {
        if (orderInfo.Pelanggan == username) {
            const Date& orderDate = orderInfo.tanggalPemesanan;
            
            if (mostRecentOrderId.empty() || 
                orderDate.year > mostRecentDate.year ||
                (orderDate.year == mostRecentDate.year && orderDate.month > mostRecentDate.month) ||
                (orderDate.year == mostRecentDate.year && orderDate.month == mostRecentDate.month && orderDate.day > mostRecentDate.day)) {
                
                mostRecentOrderId = orderId;
                mostRecentDate = orderDate;
            }
        }
    }
    
    return mostRecentOrderId;
}

void UserDB::undoLastOrder(const std::string& username) const {
    while (true) { // Loop utama untuk redisplay jika error
        clearConsole();
        showMainHeader();
        
        std::cout << "===== BATALKAN PESANAN TERAKHIR =====" << std::endl;
        
        if (userOrderHistory.find(username) == userOrderHistory.end() || 
            userOrderHistory[username].isEmpty()) {
            std::cout << "Tidak ada pesanan yang dapat dibatalkan.\n";
            std::cout << "Catatan: Hanya pesanan yang dibuat dalam sesi ini yang dapat di-undo.\n";
            std::cout << "Tekan Enter untuk kembali...";
            std::cin.ignore();
            return;
        }
        
        if (!ordersDB) {
            std::cout << "Sistem pesanan tidak tersedia.\n";
            std::cout << "Tekan Enter untuk kembali...";
            std::cin.ignore();
            return;
        }
        
        std::string lastOrderId = userOrderHistory[username].peek();
        
        const auto& allOrders = ordersDB->getOrderList();
        auto orderIt = allOrders.find(lastOrderId);
        
        if (orderIt == allOrders.end()) {
            std::cout << "Pesanan tidak ditemukan dalam sistem.\n";
            userOrderHistory[username].pop();
            std::cout << "Tekan Enter untuk kembali...";
            std::cin.ignore();
            return;
        }
        
        const OrderInfo& order = orderIt->second;
        
        // Hanya bisa cancel yang statusnya "diproses"
        if (order.status != "diproses") {
            std::cout << "Pesanan dengan status '" << order.status << "' tidak dapat dibatalkan.\n";
            std::cout << "Hanya pesanan dengan status 'diproses' yang dapat dibatalkan.\n";
            userOrderHistory[username].pop();
            std::cout << "Tekan Enter untuk kembali...";
            std::cin.ignore();
            return;
        }
        
        extern std::unordered_map<std::string, std::vector<MenuItem>> menuItems;
        
        std::cout << "Detail pesanan yang akan dibatalkan:\n";
        std::cout << "----------------------------------------\n";
        std::cout << "ID Pesanan    : " << lastOrderId << std::endl;
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
        std::cout << "----------------------------------------\n\n";
        
        std::cout << "Apakah Anda yakin ingin membatalkan pesanan ini?\n";
        std::cout << "[1] Ya, batalkan pesanan\n";
        std::cout << "[0] Tidak, kembali ke menu\n";
        std::cout << "-------------------------------------\n";
        
        int confirmChoice;
        bool inputValid = false;
        
        while (!inputValid) {
            std::cout << "Pilihan: ";
            
            if (!(std::cin >> confirmChoice)) {
                std::cout << "Input tidak valid. Masukkan angka!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pauseScreen();
                break; // Break untuk redisplay semua menu
            }
            
            std::cin.ignore();
            
            if (confirmChoice < 0 || confirmChoice > 1) {
                std::cout << "Pilihan tidak valid. Masukkan 0 atau 1!\n";
                pauseScreen();
                break; // Break untuk redisplay semua menu
            }
            
            inputValid = true;
        }
        
        // Jika input invalid, ulangi dari awal
        if (!inputValid) {
            continue;
        }
        
        if (confirmChoice == 1) {
            // Batalkan order dengan update status jadi "dibatalkan"
            ordersDB->updateOrderStatusInFile(lastOrderId, "dibatalkan");
        
            userOrderHistory[username].pop();
            
            std::cout << "\nPesanan berhasil dibatalkan!\n";
            std::cout << "ID Pesanan: " << lastOrderId << "\n";
            std::cout << "Status diubah menjadi: dibatalkan\n";
        } else {
            std::cout << "\nPembatalan pesanan dibatalkan.\n";
        }
        
        std::cout << "\nTekan Enter untuk kembali...";
        std::cin.ignore();
        return;
    }
}

void UserDB::viewOrderHistoryWithUndo(const std::string& username) const {
    clearConsole();
    showMainHeader();

    if (!ordersDB) {
        std::cout << "Sistem pesanan tidak tersedia.\n";
        return;
    }

    std::cout << "===== RIWAYAT PESANAN & UNDO (" << username << ") =====" << std::endl;

    // Tampilkan pesanan yang bisa di-undo
    if (userOrderHistory.find(username) != userOrderHistory.end() && 
        !userOrderHistory[username].isEmpty()) {
        
        std::cout << "\n=== PESANAN YANG DAPAT DIBATALKAN ===\n";
        CustomStack<std::string> tempStack = userOrderHistory[username];
        const auto& allOrders = ordersDB->getOrderList();
        
        int undoCount = 0;
        while (!tempStack.isEmpty() && undoCount < 3) {
            std::string orderId = tempStack.peek();
            tempStack.pop();
            
            auto orderIt = allOrders.find(orderId);
            if (orderIt != allOrders.end() && orderIt->second.status == "diproses") {
                std::cout << "- " << orderId << " (Status: " << orderIt->second.status << ")\n";
                undoCount++;
            }
        }
        
        if (undoCount == 0) {
            std::cout << "Tidak ada pesanan yang dapat dibatalkan.\n";
        }
        std::cout << "========================\n\n";
    }

    std::vector<std::string> customerOrderIds = ordersDB->getSortedOrderIds("", username);

    if (customerOrderIds.empty()) {
        std::cout << "Anda belum memiliki pesanan.\n";
        std::cout << "Tekan Enter untuk kembali...";
        std::cin.ignore();
        return;
    }

    const unordered_map<string, OrderInfo>& allOrders = ordersDB->getOrderList();
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
}

void UserDB::viewAllOrdersForStaff() const {
    clearConsole();
    showMainHeader();

    if (!ordersDB) {
        std::cout << "Sistem pesanan tidak tersedia.\n";
        return;
    }

    std::cout << "===== DAFTAR SEMUA PESANAN =====" << std::endl;

    std::vector<std::string> sortedOrderIds = ordersDB->getSortedOrderIds();

    if (sortedOrderIds.empty()) {
        std::cout << "Belum ada pesanan yang masuk.\n";
        std::cout << "Tekan Enter untuk kembali...";
        std::cin.ignore();
        return;
    }

    const auto& allOrders = ordersDB->getOrderList();
    extern std::unordered_map<std::string, std::vector<MenuItem>> menuItems;

    for (const auto& orderId : sortedOrderIds) {
        const auto& order = allOrders.at(orderId);
        
        // Tampilkan nama lengkap customer bukan username
        std::string customerFullName = order.Pelanggan; 
        auto userIt = userMap.find(order.Pelanggan);
        if (userIt != userMap.end()) {
            customerFullName = userIt->second.nama;
        }
        
        std::cout << "----------------------------------------\n";
        std::cout << "ID Pesanan    : " << orderId << std::endl;
        std::cout << "Pelanggan     : " << customerFullName << std::endl;
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

void UserDB::viewOrderHistoryCustomer(const std::string& username) const {
    clearConsole();
    showMainHeader();

    if (!ordersDB) {
        std::cout << "Sistem pesanan tidak tersedia.\n";
        pauseScreen();
        return;
    }

    std::cout << "===== RIWAYAT PESANAN (" << username << ") =====" << std::endl;

    std::vector<std::string> customerOrderIds = ordersDB->getSortedOrderIds("", username);

    if (customerOrderIds.empty()) {
        std::cout << "Anda belum memiliki pesanan.\n";
        std::cout << "Tekan Enter untuk kembali...";
        std::cin.ignore();
        return;
    }

    const auto& allOrders = ordersDB->getOrderList();
    extern std::unordered_map<std::string, std::vector<MenuItem>> menuItems;

    for (const auto& orderId : customerOrderIds) {
        const auto& order = allOrders.at(orderId);
        
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
    
    std::cout << "\nTekan Enter untuk kembali...";
    std::cin.ignore();
}