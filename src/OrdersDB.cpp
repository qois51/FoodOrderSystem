#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "OrderInfo.h"
#include "OrdersDB.h"

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
}