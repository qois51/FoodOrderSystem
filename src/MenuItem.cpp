#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <limits>

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

    // Main loop that handles both item selection and confirmation
    while (!done) {
        // Item selection loop
        bool itemSelectionDone = false;
        
        while (!itemSelectionDone) {
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
            bool inputValid = false;
            
            while (!inputValid) {
                std::cout << "Masukkan nomor pilihan: ";
                
                if (!(std::cin >> catChoice)) {
                    std::cout << "Input tidak valid. Masukkan angka!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    pauseScreen();
                    
                    clearConsole();
                    showMainHeader();
                    std::cout << "===== Pilih kategori menu =====\n";
                    idx = 1;
                    for (const auto& cat : menuItems) {
                        std::cout << "[" << idx << "] " << cat.first << std::endl;
                        idx++;
                    }
                    std::cout << "\n[0] Selesai memilih\n";
                    std::cout << "-------------------------------------\n";
                    continue;
                }
                
                std::cin.ignore();
                
                if (catChoice < 0 || catChoice > static_cast<int>(categories.size())) {
                    std::cout << "Kategori tidak valid.\n";
                    pauseScreen();
                    
                    clearConsole();
                    showMainHeader();
                    std::cout << "===== Pilih kategori menu =====\n";
                    idx = 1;
                    for (const auto& cat : menuItems) {
                        std::cout << "[" << idx << "] " << cat.first << std::endl;
                        idx++;
                    }
                    std::cout << "\n[0] Selesai memilih\n";
                    std::cout << "-------------------------------------\n";
                    continue;
                }
                
                inputValid = true;
            }

            if (catChoice == 0) {
                itemSelectionDone = true;
                continue;
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
                bool itemInputValid = false;
                
                while (!itemInputValid) {
                    std::cout << "Pilih nomor menu: ";
                    
                    if (!(std::cin >> itemNumber)) {
                        std::cout << "Input tidak valid. Masukkan angka!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        pauseScreen();
                        
                        clearConsole();
                        showMainHeader();
                        std::cout << "==== Menu " << category << " ====\n";
                        for (size_t i = 0; i < items.size(); ++i) {
                            std::cout << "[" << (i + 1) << "] " << items[i].name << " - Rp" << items[i].price << "\n";
                        }
                        std::cout << "\n[0] Kembali ke kategori\n";
                        std::cout << "-------------------------------------\n";
                        continue;
                    }
                    
                    std::cin.ignore();
                    
                    if (itemNumber < 0 || itemNumber > static_cast<int>(items.size())) {
                        std::cout << "Nomor menu tidak valid.\n";
                        pauseScreen();
                        
                        clearConsole();
                        showMainHeader();
                        std::cout << "==== Menu " << category << " ====\n";
                        for (size_t i = 0; i < items.size(); ++i) {
                            std::cout << "[" << (i + 1) << "] " << items[i].name << " - Rp" << items[i].price << "\n";
                        }
                        std::cout << "\n[0] Kembali ke kategori\n";
                        std::cout << "-------------------------------------\n";
                        continue;
                    }
                    
                    itemInputValid = true;
                }

                if (itemNumber == 0) {
                    break;
                }

                int quantity;
                bool quantityInputValid = false;
                
                while (!quantityInputValid) {
                    std::cout << "Masukkan jumlah: ";
                    
                    if (!(std::cin >> quantity)) {
                        std::cout << "Input tidak valid. Masukkan angka!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        pauseScreen();
                        continue;
                    }
                    
                    std::cin.ignore();
                    
                    if (quantity <= 0) {
                        std::cout << "Jumlah harus lebih dari 0.\n";
                        pauseScreen();
                        continue;
                    }
                    
                    quantityInputValid = true;
                }

                std::string menuName = items[itemNumber - 1].name;
                selectedItems[menuName] += quantity;

                std::cout << "Pesanan Berhasil ditambahkan\n";
                delay(1);
            }
        }

        // Confirmation section
        bool confirmationDone = false;
        while (!confirmationDone) {
            clearConsole();
            showMainHeader();

            std::cout << "===== Konfirmasi Pesanan =====\n";
            if (selectedItems.empty()) {
                std::cout << "Anda belum memilih item apapun.\n";
                std::cout << "[1] Tambah item\n";
                std::cout << "[0] Batalkan pesanan\n";
                std::cout << "----------------------------\n"; 
                
                int confirmChoice;
                bool confirmInputValid = false;
                
                while (!confirmInputValid) {
                    std::cout << "Pilih menu: ";
                    
                    if (!(std::cin >> confirmChoice)) {
                        std::cout << "Input tidak valid. Masukkan angka!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        pauseScreen();
                        
                        clearConsole();
                        showMainHeader();
                        std::cout << "===== Konfirmasi Pesanan =====\n";
                        std::cout << "Anda belum memilih item apapun.\n";
                        std::cout << "[1] Tambah item\n";
                        std::cout << "[0] Batalkan pesanan\n";
                        std::cout << "----------------------------\n";
                        continue;
                    }
                    
                    std::cin.ignore();
                    
                    if (confirmChoice < 0 || confirmChoice > 1) {
                        std::cout << "Pilihan tidak valid.\n";
                        pauseScreen();
                        
                        clearConsole();
                        showMainHeader();
                        std::cout << "===== Konfirmasi Pesanan =====\n";
                        std::cout << "Anda belum memilih item apapun.\n";
                        std::cout << "[1] Tambah item\n";
                        std::cout << "[0] Batalkan pesanan\n";
                        std::cout << "----------------------------\n";
                        continue;
                    }
                    
                    confirmInputValid = true;
                }

                if (confirmChoice == 0) {
                    std::cout << "Pesanan dibatalkan.\n";
                    selectedItems.clear();
                    delay(1);
                    return selectedItems;
                } else if (confirmChoice == 1) {
                    confirmationDone = true; // Go back to item selection
                }
            } else {
                int idx = 1;
                int totalPrice = 0;

                std::cout << "Daftar pesanan Anda:\n";
                std::cout << "----------------------------\n";

                for (const auto& [menuName, qty] : selectedItems) {
                    // Find the item price in the menu
                    int itemPrice = 0;
                    for (const auto& [category, items] : menuItems) {
                        for (const auto& item : items) {
                            if (item.name == menuName) {
                                itemPrice = item.price;
                                break;
                            }
                        }
                        if (itemPrice > 0) break;
                    }
                
                    int subtotal = itemPrice * qty;
                    totalPrice += subtotal;
                
                    std::cout << idx++ << ". " << menuName 
                            << " (Rp" << itemPrice << " x " << qty 
                            << ") = Rp" << subtotal << "\n";
                }
            
                std::cout << "----------------------------\n";
                std::cout << "TOTAL HARGA: Rp" << totalPrice << "\n";
                std::cout << "----------------------------\n";

                std::cout << std::endl << "Aksi: \n";
                std::cout << "[1] Konfirmasi pesanan\n";
                std::cout << "[2] Tambah item lagi\n";
                std::cout << "[0] Batalkan pesanan\n";
                std::cout << "----------------------------\n";

                int confirmChoice;
                bool finalConfirmValid = false;
                
                while (!finalConfirmValid) {
                    std::cout << "Pilih menu: ";
                    
                    if (!(std::cin >> confirmChoice)) {
                        std::cout << "Input tidak valid. Masukkan angka!\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        pauseScreen();
                        
                        // Redisplay order summary (same code as above)
                        clearConsole();
                        showMainHeader();
                        std::cout << "===== Konfirmasi Pesanan =====\n";
                        std::cout << "Daftar pesanan Anda:\n";
                        std::cout << "----------------------------\n";
                        
                        idx = 1;
                        totalPrice = 0;
                        for (const auto& [menuName, qty] : selectedItems) {
                            int itemPrice = 0;
                            for (const auto& [category, items] : menuItems) {
                                for (const auto& item : items) {
                                    if (item.name == menuName) {
                                        itemPrice = item.price;
                                        break;
                                    }
                                }
                                if (itemPrice > 0) break;
                            }
                            int subtotal = itemPrice * qty;
                            totalPrice += subtotal;
                            std::cout << idx++ << ". " << menuName 
                                    << " (Rp" << itemPrice << " x " << qty 
                                    << ") = Rp" << subtotal << "\n";
                        }
                        
                        std::cout << "----------------------------\n";
                        std::cout << "TOTAL HARGA: Rp" << totalPrice << "\n";
                        std::cout << "----------------------------\n";
                        std::cout << std::endl << "Aksi: \n";
                        std::cout << "[1] Konfirmasi pesanan\n";
                        std::cout << "[2] Tambah item lagi\n";
                        std::cout << "[0] Batalkan pesanan\n";
                        std::cout << "----------------------------\n";
                        continue;
                    }
                    
                    std::cin.ignore();
                    
                    if (confirmChoice < 0 || confirmChoice > 2) {
                        std::cout << "Pilihan tidak valid.\n";
                        pauseScreen();
                        
                        // Redisplay order summary (same code as above)
                        clearConsole();
                        showMainHeader();
                        std::cout << "===== Konfirmasi Pesanan =====\n";
                        std::cout << "Daftar pesanan Anda:\n";
                        std::cout << "----------------------------\n";
                        
                        idx = 1;
                        totalPrice = 0;
                        for (const auto& [menuName, qty] : selectedItems) {
                            int itemPrice = 0;
                            for (const auto& [category, items] : menuItems) {
                                for (const auto& item : items) {
                                    if (item.name == menuName) {
                                        itemPrice = item.price;
                                        break;
                                    }
                                }
                                if (itemPrice > 0) break;
                            }
                            int subtotal = itemPrice * qty;
                            totalPrice += subtotal;
                            std::cout << idx++ << ". " << menuName 
                                    << " (Rp" << itemPrice << " x " << qty 
                                    << ") = Rp" << subtotal << "\n";
                        }
                        
                        std::cout << "----------------------------\n";
                        std::cout << "TOTAL HARGA: Rp" << totalPrice << "\n";
                        std::cout << "----------------------------\n";
                        std::cout << std::endl << "Aksi: \n";
                        std::cout << "[1] Konfirmasi pesanan\n";
                        std::cout << "[2] Tambah item lagi\n";
                        std::cout << "[0] Batalkan pesanan\n";
                        std::cout << "----------------------------\n";
                        continue;
                    }
                    
                    finalConfirmValid = true;
                }
                
                if (confirmChoice == 1) {
                    std::cout << "Pesanan Anda telah dikonfirmasi.\n";
                    delay(1);
                    confirmationDone = true;
                    done = true; // Exit the main loop
                } else if (confirmChoice == 2) {
                    confirmationDone = true; // Exit confirmation loop, go back to item selection
                    // done remains false, so main loop continues
                } else if (confirmChoice == 0) {
                    std::cout << "Pesanan dibatalkan.\n";
                    delay(1);
                    selectedItems.clear();
                    confirmationDone = true;
                    done = true; // Exit the main loop
                }
            }
        }
    }
    
    return selectedItems;
}