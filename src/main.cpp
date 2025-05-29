#include <iostream>
#include <limits>
#include <fstream>

#include "UserDB.h"
#include "OrdersDB.h"
#include "Clear.h"
#include "headers.h"

int main() {
    std::vector<std::string> possiblePaths = {
        "../data/",            
        "./data/",             
        "data/",               
    };
    
    std::string orderDataPath;
    std::string userDataPath;
    bool filesFound = false;
    
    if (!filesFound) {
        for (const auto& path : possiblePaths) {
            std::string testPath = path + "orderData.csv";
            std::ifstream testFile(testPath);
            if (testFile.good()) {
                orderDataPath = testPath;
                userDataPath = path + "userData.csv";
                filesFound = true;
                testFile.close();
                break;
            }
        }
    }
    
    if (!filesFound) {
        std::cerr << "Error: Could not locate data files in any of the expected locations." << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.ignore();
        return 1;
    }

    OrdersDB ordersDB(orderDataPath);
    UserDB users(userDataPath, &ordersDB);

    while (true) {
        clearConsole();
        showMainHeader();
        std::cout << " [1] Login\n";
        std::cout << " [2] Register\n";
        std::cout << " [3] Lupa Password\n";
        std::cout << " [0] Keluar\n";
        std::cout << "----------------------------------\n";
        
        int choice;
        bool inputValid = false;
        
        while (!inputValid) {
            std::cout << "Pilih menu: ";
        
            if (!(std::cin >> choice)) {
                std::cout << "Input tidak valid. Masukkan angka!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                pauseScreen();

                clearConsole();
                showMainHeader();
                std::cout << " [1] Login\n";
                std::cout << " [2] Register\n";
                std::cout << " [3] Lupa Password\n";
                std::cout << " [0] Keluar\n";
                std::cout << "----------------------------------\n";
                continue;
            }
        
            std::cin.ignore();
            inputValid = true;
        }

        switch(choice) {
            case 1: {
                std::string username = users.login();
                if (!username.empty()) {
                    std::cout << "\n[SUKSES] Login berhasil! Selamat datang, " << username << "!\n";
                    delay(1);
                    users.displayUserActivities(username);
                } else {
                    std::cout << "\n[GAGAL] Gagal login, silakan coba lagi.\n";
                    pauseScreen();
                }
                break;
            }
            case 2: {
                clearConsole();
                users.showRegistrationUI();
                break;
            }
            case 3: {
                clearConsole();
                users.showResetPasswordUI();
                break;
            }
            case 0:
                std::cout << "\nTerima kasih telah menggunakan sistem.\n";
                return 0;
                break;
            default:
                std::cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
                pauseScreen();
                break;
        }
    }
}