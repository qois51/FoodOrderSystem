#pragma once
#include <string>
#include <unordered_map>

struct MenuItem {
    std::string name;
    double price;

    MenuItem(const std::string& name, double price)
        : name(name), price(price) {}
};

extern std::unordered_map<std::string, std::vector<MenuItem>> menuItems;

std::unordered_map<std::string, int> selectMenuItems();