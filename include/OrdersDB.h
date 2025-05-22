#pragma once
#include <string>
#include <unordered_map>
#include "OrderInfo.h"

using namespace std;

class OrdersDB {
private:
    unordered_map<string, OrderInfo> orderList;
    string dbFilePath;
    

public:
    OrdersDB(string filelocation);
    const unordered_map<string, OrderInfo>& getOrderList() const;
    string generateOrderID();
    void createNewOrder(const std::string& username);
    void writeToFile(OrderInfo& order, const std::string& orderId);
    void updateOrderStatusInFile(const std::string& orderId, const std::string& newStatus);
    std::vector<std::string> getSortedOrderIds(const std::string& status);
    void processOrder();
};