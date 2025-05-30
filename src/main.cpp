#include <iostream>
#include <limits>
#include <filesystem>

#include "UserDB.h"
#include "OrdersDB.h"
#include "Clear.h"
#include "headers.h"

std::string findDataFile(const std::string& filename) {
    std::vector<std::string> possiblePaths = {
        "./data/" + filename, 
        "../data/" + filename,
    };
    
    for (const auto& path : possiblePaths) {
        if (std::filesystem::exists(path)) {
            std::cout << "Data file found: " << std::filesystem::absolute(path) << std::endl;
            return path;
        }
    }
    
    std::cout << "Data file not found, using default: ./data/" << filename << std::endl;
    return "./data/" + filename;
}

int main() {
    std::string orderDataPath = findDataFile("orderData.csv");
    std::string userDataPath = findDataFile("userData.csv");
    
    OrdersDB orders(orderDataPath);
    UserDB users(userDataPath, &orders);

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
                std::cout << "\nInput tidak valid. Masukkan angka!\n";
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
                    std::cout << "\n[SUKSES] Login berhasil! Selamat datang, " << username << "! ";
                    delay(2);
                    users.displayUserActivities(username);
                } else {
                    std::cout << "[GAGAL] Gagal login, silakan coba lagi.\n";
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
                pauseScreen();
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