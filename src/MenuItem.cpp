#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include "MenuItem.h"

std::unordered_map<std::string, std::vector<MenuItem>> menuItems = {
    {"Makanan", {
        {"Nasi Goreng", 20000},
        {"Sate Ayam", 25000},
        {"Rendang Daging", 30000}
    }},
    {"Minuman", {
        {"Es Teh Manis", 5000},
        {"Kopi Hitam", 10000},
        {"Jus Jeruk", 15000}
    }},
    {"Snack", {
        {"Keripik Singkong", 7000},
        {"Pisang Goreng", 8000},
        {"Tahu Tempe", 6000}
    }}
};

std::unordered_map<std::string, int> selectMenuItems() {
    std::unordered_map<std::string, int> selectedItems;
    char more = 'y';

    while (more == 'y' || more == 'Y') {
        // Display categories
        std::cout << "\nPilih kategori menu:\n";
        std::vector<std::string> categories;
        int idx = 1;
        for (const auto& cat : menuItems) {
            std::cout << idx << ". " << cat.first << std::endl;
            categories.push_back(cat.first);
            idx++;
        }

        int catChoice;
        std::cout << "Masukkan nomor kategori: ";
        std::cin >> catChoice;
        std::cin.ignore();

        if (catChoice < 1 || catChoice > categories.size()) {
            std::cout << "Kategori tidak valid.\n";
            continue;
        }
        std::string category = categories[catChoice - 1];

        // Display menu in selected category
        const auto& items = menuItems[category];
        std::cout << "\nMenu " << category << ":\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << (i + 1) << ". " << items[i].name << " - Rp" << items[i].price << "\n";
        }

        int itemNumber, quantity;
        std::cout << "Pilih nomor menu: ";
        std::cin >> itemNumber;
        std::cout << "Masukkan jumlah: ";
        std::cin >> quantity;
        std::cin.ignore();

        if (itemNumber > 0 && itemNumber <= items.size()) {
            std::string menuName = items[itemNumber - 1].name;
            selectedItems[menuName] += quantity;
        } else {
            std::cout << "Nomor menu tidak valid.\n";
        }

        std::cout << "Tambah item lain? (y/n): ";
        std::cin >> more;
        std::cin.ignore();
    }
    return selectedItems;
}