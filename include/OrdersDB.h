#pragma once
#include <string>
#include <unordered_map>
#include "OrderInfo.h"

using namespace std;

class OrdersDB {
private:
    unordered_map<string, OrderInfo> orderList;
public:
    OrdersDB(string filelocation);
};