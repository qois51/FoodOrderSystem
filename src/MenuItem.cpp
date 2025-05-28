#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

#include "MenuItem.h"
#include "Clear.h"
#include "headers.h" 

std::unordered_map<std::string, std::vector<MenuItem>> menuItems = {
    {"Camilan", {
        {"Kentang Goreng", 8000},
        {"Bawang Goreng Crispy", 10000},
        {"Nugget Ayam", 12000},
        {"Sandwich", 15000}
    }},
    {"Minuman", {
        {"Coca-Cola", 5000},
        {"Fanta", 5000},
        {"Air Mineral", 3000},
        {"Lemon Tea", 7000},
        {"Milkshake", 12000},
        {"Jus Jeruk", 80000}
    }},
    {"Makanan", {
        {"Burger", 15000},
        {"Pizza", 25000},
        {"Ayam Goreng", 20000},
        {"Hot Dog", 13000},
        {"Spaghetti Bolognese", 20000}
    }}
};

std::unordered_map<std::string, int> selectMenuItems() {
    std::unordered_map<std::string, int> selectedItems;
    bool done = false;

    while (!done) {
        clearConsole();
        showMainHeader();

        std::cout << "===== Pilih kategori menu =====\n";
        std::vector<std::string> categories;
        int idx = 1;
        for (const auto& cat : menuItems) {
            std::cout << "[" << idx << "] " << cat.first << std::endl;
            categories.push_back(cat.first);
            idx++;
        }
        std::cout << "\n[0] Selesai memilih\n";
        std::cout << "-------------------------------------\n";

        int catChoice;
        while (true) {
            std::cout << "Masukkan nomor pilihan: ";
            if (!(std::cin >> catChoice)) {
                std::cout << "Pilihan tidak valid.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                pauseScreen();
                continue;
            }
            std::cin.ignore();
            if (catChoice < 0 || catChoice > categories.size()) {
                std::cout << "Kategori tidak valid.\n";
                pauseScreen();
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
            showMainHeader();

            const auto& items = menuItems[category];
            std::cout << "==== Menu " << category << " ====\n";
            for (size_t i = 0; i < items.size(); ++i) {
                std::cout << "[" << (i + 1) << "] " << items[i].name << " - Rp" << items[i].price << "\n";
            }
            std::cout << "\n[0] Kembali ke kategori\n";
            std::cout << "-------------------------------------\n";

            int itemNumber;
            while (true) {
                std::cout << "Pilih nomor menu: ";
                if (!(std::cin >> itemNumber)) {
                    std::cout << "Pilihan tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    pauseScreen();
                    continue;
                }
                std::cin.ignore();
                if (itemNumber < 0 || itemNumber > static_cast<int>(items.size())) {
                    std::cout << "Nomor menu tidak valid.\n";
                    pauseScreen();
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

            std::cout << "Pesanan Berhasil ditambahkan\n";
            delay(1);
        }
    }

    while (true) {
        clearConsole();
        showMainHeader();

        std::cout << "===== Konfirmasi Pesanan =====\n";
        if (selectedItems.empty()) {
            std::cout << "Anda belum memilih item apapun.\n";
            std::cout << "[1] Tambah item\n";
            std::cout << "[0] Batalkan pesanan\n";
            std::cout << "----------------------------\n"; 
            int confirmChoice;
            while (true) {
                std::cout << "Pilih menu: ";
                if (!(std::cin >> confirmChoice)) {
                    std::cout << "Pilihan tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    pauseScreen();
                    continue;
                }
                std::cin.ignore();
                if (confirmChoice != 0) {
                    std::cout << "Pilihan tidak valid.\n";
                    pauseScreen();
                    continue;
                }
                break;
            }
            if (confirmChoice == 0) selectedItems.clear();
            else if (confirmChoice == 1) {
                done = false; 
                continue;
            };
            
            break;
        } else {
            int idx = 1;
            std::cout << "Daftar pesanan Anda:\n";
            for (const auto& [menuName, qty] : selectedItems) {
                std::cout << idx++ << ". " << menuName << " x" << qty << "\n";
            }
            std::cout << std::endl << "Aksi: \n";
            std::cout << "[1] Konfirmasi pesanan\n";
            std::cout << "[2] Tambah item lagi\n";
            std::cout << "[0] Batalkan pesanan\n";
            std::cout << "----------------------------\n";

            int confirmChoice;
            while (true) {
                std::cout << "Pilih menu: ";
                if (!(std::cin >> confirmChoice)) {
                    std::cout << "Pilihan tidak valid.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    pauseScreen();
                    continue;
                }
                std::cin.ignore();
                if (confirmChoice < 0 || confirmChoice > 2) {
                    std::cout << "Pilihan tidak valid.\n";
                    pauseScreen();
                    continue;
                }
                break;
            }
            if (confirmChoice == 1) {
                std::cout << "Pesanan Anda telah dikonfirmasi.\n";
                done = true;
                delay(1);
                break; 
            } else if (confirmChoice == 2) {
                done = false; 
                continue;
            } else if (confirmChoice == 0) {
                std::cout << "Pesanan dibatalkan.\n";
                delay(1);
                selectedItems.clear();
                break;
            }
        }
    }
    return selectedItems;
}