#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <random>
#include <limits>

#include "OrderInfo.h"
#include "OrdersDB.h"
#include "Date.h"
#include "MenuItem.h"
#include "Queque.h"
#include "Clear.h"
#include "headers.h"

using namespace std;

OrdersDB::OrdersDB(string filelocation)
    : dbFilePath(filelocation) {

    string line;
    ifstream file(dbFilePath);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << filelocation << endl;
        return;
    }

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string orderId, dateStr, status, item, pelanggan;
        int quantity;

        getline(ss, orderId, ',');
        getline(ss, dateStr, ',');
        getline(ss, status, ',');
        getline(ss, item, ',');

        string qtyStr;
        getline(ss, qtyStr, ',');
        quantity = stoi(qtyStr);

        getline(ss, pelanggan, ',');

        // Parse date
        int year, month, day;
        sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);

        // Create a new order if it doesn't exist
        if (orderList.find(orderId) == orderList.end()) {
            OrderInfo newOrder;
            newOrder.tanggalPemesanan = {year, month, day};
            newOrder.status = status;
            newOrder.Pelanggan = pelanggan;
            orderList[orderId] = newOrder;
        }

        orderList[orderId].itemPesanan[item] = quantity;
    }

    cout << "Loaded data from: " << filesystem::absolute(dbFilePath) << endl;
}

const unordered_map<string, OrderInfo>& OrdersDB::getOrderList() const {
    return orderList;
}

string OrdersDB::generateOrderID() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << "ORD"
        << std::put_time(&tm, "%Y%m%d-%H%M%S") << "-";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    oss << dis(gen);

    return oss.str();
}

void OrdersDB::createNewOrder(const std::string& username) {
    auto selectedItems = selectMenuItems();
    if (selectedItems.empty()) {
        return;
    }

    string orderId = OrdersDB::generateOrderID();
    Date today = getCurrentDate();

    OrderInfo newOrder;
    newOrder.Pelanggan = username;
    newOrder.tanggalPemesanan = today;
    newOrder.status = "diproses";

    for (const auto& [itemName, quantity] : selectedItems) {
        newOrder.itemPesanan[itemName] = quantity;
    }

    orderList[orderId] = newOrder;

    writeToFile(newOrder, orderId);
}

void OrdersDB::writeToFile(OrderInfo& order, const string& orderId) {
    ofstream file(dbFilePath, ios::app);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << dbFilePath << endl;
        return;
    }

    string dateStr = dateToString(order.tanggalPemesanan);

    std::ifstream checkFile(dbFilePath, std::ios::binary);
    bool needsNewline = false;
    if (checkFile.is_open()) {
        checkFile.seekg(-1, std::ios::end);
        char lastChar;
        checkFile.get(lastChar);
        if (lastChar != '\n') needsNewline = true;
        checkFile.close();
    }
    if (needsNewline) file << std::endl;
    
    for (const auto& [item, quantity] : order.itemPesanan) {
        file << orderId << ","
             << dateStr << ","
             << order.status << ","
             << item << ","
             << quantity << ","
             << order.Pelanggan << endl;
    }
}

void OrdersDB::updateOrderStatusInFile(const std::string& orderId, const std::string& newStatus) {
    std::vector<std::string> lines;
    std::ifstream inFile(dbFilePath);
    std::string line;
    
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string currentOrderId;
        std::getline(ss, currentOrderId, ',');
        
        if (currentOrderId == orderId) {
            std::stringstream updatedLine;
            std::string dateStr, oldStatus, item, quantity, customer;
            
            std::getline(ss, dateStr, ',');
            std::getline(ss, oldStatus, ',');
            std::getline(ss, item, ',');
            std::getline(ss, quantity, ',');
            std::getline(ss, customer);
            
            updatedLine << orderId << "," 
                       << dateStr << "," 
                       << newStatus << ","
                       << item << "," 
                       << quantity << "," 
                       << customer;
            
            lines.push_back(updatedLine.str());
        } else {
            lines.push_back(line);
        }
    }
    inFile.close();
    
    std::ofstream outFile(dbFilePath);
    for (const auto& l : lines) {
        outFile << l << std::endl;
    }
    outFile.close();
}

void OrdersDB::mergeSort(std::vector<std::string>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void OrdersDB::merge(std::vector<std::string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::string> L(n1);
    std::vector<std::string> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}   

std::vector<std::string> OrdersDB::getSortedOrderIds(const std::string& status, const std::string& username) {
    std::vector<std::string> orderIds;
    
    for (const auto& [orderId, order] : orderList) {
        // Filter by both status and username (Opsional)
        bool statusMatch = status.empty() || order.status == status;
        bool usernameMatch = username.empty() || order.Pelanggan == username;
        
        if (statusMatch && usernameMatch) {
            orderIds.push_back(orderId);
        }
    }
    
    // Use Merge Sort
    if (!orderIds.empty()) {
        mergeSort(orderIds, 0, orderIds.size() - 1);
    }

    return orderIds;
}

void OrdersDB::processOrder() {
    std::vector<string> orderIds = getSortedOrderIds("diproses");
    
    if (orderIds.empty()) {
        std::cout << "\nTidak ada pesanan dengan status 'diproses'.\n";
        pauseScreen();
        return;
    }

    CustomQueue<string> orderQueue;
    for (const auto& orderId : orderIds) {
        orderQueue.enqueue(orderId);
    }

    while (!orderQueue.isEmpty()) {
        clearConsole();
        showMainHeader();
        std::cout << "========== DAFTAR ANTRIAN ORDER ==========\n";
        int idx = 1;
        for (const auto& orderId : orderIds) {
            const auto& order = orderList.at(orderId);
            if (order.status == "diproses") { 
                std::cout << idx++ << ". Order ID: " << orderId
                        << ", Pelanggan: " << order.Pelanggan
                        << ", Tanggal: " << order.tanggalPemesanan.year << "-"
                        << order.tanggalPemesanan.month << "-"
                        << order.tanggalPemesanan.day << std::endl;
                      
                std::cout << "   Item: ";
                for (const auto& [item, qty] : order.itemPesanan) {
                    std::cout << item << " (" << qty << "), ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << "========================================\n\n";

        // Get the next order from queue
        std::string currentOrderId = orderQueue.peek();
        const auto& currentOrder = orderList.at(currentOrderId);
        
        std::cout << "PESANAN: " << currentOrderId << " - " << currentOrder.Pelanggan << std::endl;
        std::cout << "[1] Tandai sebagai selesai\n";
        std::cout << "[2] Batalkan pesanan\n";
        std::cout << "[0] Kembali ke menu sebelumnya\n";
        std::cout << "--------------------------------------\n";
        
        int statusChoice;
        bool inputValid = false;
        
        // Input validation loop
        while (!inputValid) {
            std::cout << "Pilihan: ";
            
            if (!(std::cin >> statusChoice)) {
                std::cout << "Input tidak valid. Masukkan angka!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pauseScreen();
                clearConsole();
                showMainHeader();
                std::cout << "========== DAFTAR ANTRIAN ORDER ==========\n";
                int idx = 1;
                for (const auto& orderId : orderIds) {
                    const auto& order = orderList.at(orderId);
                    if (order.status == "diproses") { 
                        std::cout << idx++ << ". Order ID: " << orderId
                                << ", Pelanggan: " << order.Pelanggan
                                << ", Tanggal: " << order.tanggalPemesanan.year << "-"
                                << order.tanggalPemesanan.month << "-"
                                << order.tanggalPemesanan.day << std::endl;
                              
                        std::cout << "   Item: ";
                        for (const auto& [item, qty] : order.itemPesanan) {
                            std::cout << item << " (" << qty << "), ";
                        }
                        std::cout << std::endl;
                    }
                }
                std::cout << "========================================\n\n";
                std::cout << "PESANAN: " << currentOrderId << " - " << currentOrder.Pelanggan << std::endl;
                std::cout << "[1] Tandai sebagai selesai\n";
                std::cout << "[2] Batalkan pesanan\n";
                std::cout << "[0] Kembali ke menu sebelumnya\n";
                std::cout << "--------------------------------------\n";
                continue;
            }
            
            std::cin.ignore();
            inputValid = true;
        }
        
        if (statusChoice == 0) {
            break; 
        }
        
        if (statusChoice == 1) {
            orderList[currentOrderId].status = "selesai";
            updateOrderStatusInFile(currentOrderId, "selesai");

            std::cout << "STATUS pesanan " << currentOrderId << " diubah menjadi 'SELESAI'.\n";
            orderQueue.dequeue();
        } else if (statusChoice == 2) {
            orderList[currentOrderId].status = "dibatalkan";
            updateOrderStatusInFile(currentOrderId, "dibatalkan");
            std::cout << "STATUS pesanan " << currentOrderId << " diubah menjadi 'DIBATALKAN'.\n";
            orderQueue.dequeue();
        } else {
            std::cout << "Pilihan tidak valid.\n";
            pauseScreen();
            continue;
        }
        
        if (orderQueue.isEmpty()) {
            std::cout << "Semua pesanan telah diproses.\n";
            break;
        }
        
        delay(2);
    }
}