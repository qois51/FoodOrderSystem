#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include "MenuItem.h"
#include "Clear.h"

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
    bool done = false;

    while (!done) {
        clearConsole();

        std::cout << "\nPilih kategori menu:\n";
        std::vector<std::string> categories;
        int idx = 1;
        for (const auto& cat : menuItems) {
            std::cout << idx << ". " << cat.first << std::endl;
            categories.push_back(cat.first);
            idx++;
        }
        std::cout << "0. Selesai memilih\n";

        int catChoice;
        while (true) {
            std::cout << "Masukkan nomor pilihan: ";
            if (!(std::cin >> catChoice)) {
                std::cout << "Pilihan tidak valid.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }
            std::cin.ignore();
            if (catChoice < 0 || catChoice > categories.size()) {
                std::cout << "Kategori tidak valid.\n";
                continue;
            }
            break;
        }

        if (catChoice == 0) {
            done = true;
            break;
        }
        std::string category = categories[catChoice - 1];

        while (true) {
            clearConsole();

            const auto& items = menuItems[category];
            std::cout << "\nMenu " << category << ":\n";
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << (i + 1) << ". " << items[i].name << " - Rp" << items[i].price << "\n";
            }
            std::cout << "0. Kembali ke kategori\n";

            int itemNumber;
            while (true) {
                std::cout << "Pilih nomor menu: ";
                if (!(std::cin >> itemNumber)) {
                    std::cout << "Pilihan tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                std::cin.ignore();
                if (itemNumber < 0 || itemNumber > static_cast<int>(items.size())) {
                    std::cout << "Nomor menu tidak valid.\n";
                    continue;
                }
                break;
            }

            if (itemNumber == 0) {
                break;
            }

            int quantity;
            while (true) {
                std::cout << "Masukkan jumlah: ";
                if (!(std::cin >> quantity)) {
                    std::cout << "Pilihan tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                std::cin.ignore();
                if (quantity <= 0) {
                    std::cout << "Jumlah harus lebih dari 0.\n";
                    continue;
                }
                break;
            }

            std::string menuName = items[itemNumber - 1].name;
            selectedItems[menuName] += quantity;
        }
    }

    while (true) {
        std::cout << "\n===== Konfirmasi Pesanan =====\n";
        if (selectedItems.empty()) {
            std::cout << "Anda belum memilih item apapun.\n";
            std::cout << "0. Batalkan pesanan\n";
            int confirmChoice;
            while (true) {
                std::cout << "Pilih menu: ";
                if (!(std::cin >> confirmChoice)) {
                    std::cout << "Pilihan tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                std::cin.ignore();
                if (confirmChoice != 0) {
                    std::cout << "Pilihan tidak valid.\n";
                    continue;
                }
                break;
            }
            if (confirmChoice == 0) selectedItems.clear();
            break;
        } else {
            int idx = 1;
            std::cout << "Daftar pesanan Anda:\n";
            for (const auto& [menuName, qty] : selectedItems) {
                std::cout << idx++ << ". " << menuName << " x" << qty << "\n";
            }
            std::cout << std::endl << "Aksi: \n";
            std::cout << "1. Konfirmasi pesanan\n";
            std::cout << "2. Tambah item lagi\n";
            std::cout << "0. Batalkan pesanan\n";

            int confirmChoice;
            while (true) {
                std::cout << "Pilih menu: ";
                if (!(std::cin >> confirmChoice)) {
                    std::cout << "Pilihan tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    continue;
                }
                std::cin.ignore();
                if (confirmChoice < 0 || confirmChoice > 2) {
                    std::cout << "Pilihan tidak valid.\n";
                    continue;
                }
                break;
            }
            if (confirmChoice == 1) {
                break; 
            } else if (confirmChoice == 2) {
                done = false; 
                continue;
            } else if (confirmChoice == 0) {
                selectedItems.clear();
                break;
            }
        }
    }
    return selectedItems;
}