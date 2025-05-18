#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <random>

#include "OrderInfo.h"
#include "OrdersDB.h"
#include "Date.h"
#include "MenuItem.h"

using namespace std;

OrdersDB::OrdersDB(string filelocation)
    : dbFilePath((std::filesystem::path("..") / filelocation).string()) {

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
    string orderId = OrdersDB::generateOrderID();
    Date today = getCurrentDate();

    OrderInfo newOrder;
    newOrder.Pelanggan = username;
    newOrder.tanggalPemesanan = today;
    newOrder.status = "diproses";

    auto selectedItems = selectMenuItems();
    for (const auto& [itemName, quantity] : selectedItems) {
        newOrder.itemPesanan[itemName] = quantity;
    }

    orderList[orderId] = newOrder;
    cout << "New order created with ID: " << orderId << endl;

    writeToFile(newOrder, orderId);
}

void OrdersDB::writeToFile(OrderInfo& order, const string& orderId) {
    ofstream file(dbFilePath, ios::app);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << dbFilePath << endl;
        return;
    }

    string dateStr = dateToString(order.tanggalPemesanan);

    // Robust: ensure new line before writing if file does not end with newline
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