#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "OrderInfo.h"
#include "OrdersDB.h"

using namespace std;

OrdersDB::OrdersDB(string filelocation) {
    string line;
    ifstream file(filelocation);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << filelocation << endl;
        return;
    }

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string orderId, dateStr, status, item;
        int quantity;

        getline(ss, orderId, ',');
        getline(ss, dateStr, ',');
        getline(ss, status, ',');
        getline(ss, item, ',');

        string qtyStr;
        getline(ss, qtyStr, ',');
        quantity = stoi(qtyStr);

        // Parse date
        int year, month, day;
        sscanf(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);

        // Check orderId sudah ada?
        if (orderList.find(orderId) == orderList.end()) {
            OrderInfo newOrder;
            newOrder.tanggalPemesanan = {year, month, day};
            newOrder.status = status;
            orderList[orderId] = newOrder;
        }

        orderList[orderId].itemPesanan[item] = quantity;
    }

    // ✅ Simple output to check
    std::cout << "Loaded Orders:\n";
    for (const auto& [orderId, order] : orderList) {
        std::cout << "Order ID: " << orderId << "\n";
        std::cout << "  Date: " << order.tanggalPemesanan.year << "-"
                  << order.tanggalPemesanan.month << "-"
                  << order.tanggalPemesanan.day << "\n";
        std::cout << "  Status: " << order.status << "\n";
        for (const auto& [itemName, qty] : order.itemPesanan) {
            std::cout << "    " << itemName << " x" << qty << "\n";
        }
        std::cout << std::endl;
    }
}