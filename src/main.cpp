#include <iostream>
#include <limits>
#include "UserDB.h"
#include "OrdersDB.h"

#include "Clear.h"
#include "headers.h"

int main() {
    OrdersDB orders("data/orderData.csv");
    UserDB users("data/userData.csv", &orders);

    while (true) {
        clearConsole();
        showMainHeader();
        std::cout << "[1] Login\n";
        std::cout << "[2] Register\n";
        std::cout << "[3] Lupa Password\n";
        std::cout << "[0] Keluar\n";
        std::cout << "\n----------------------------------\n";
        
        int choice;
        std::cout << "Pilih menu: ";
        
        // Input validation
        while (!(std::cin >> choice)) {
            std::cout << "Input tidak valid. Masukkan angka: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        std::cin.ignore();

        switch(choice) {
            case 1: {
                std::string username = users.login();
                if (!username.empty()) {
                    std::cout << "\n[SUKSES] Login berhasil! Selamat datang, " << username << "!\n";
                    pauseScreen();
                    users.displayUserActivities(username);
                } else {
                    std::cout << "\n[GAGAL] Gagal login, silakan coba lagi.\n";
                    pauseScreen();
                }
                break;
            }
            case 2: {
                clearConsole();
                std::cout << "\n+===========================================+\n";
                std::cout << "|                 REGISTER                  |\n";
                std::cout << "+===========================================+\n\n";
                
                users.showRegistrationUI();
                pauseScreen();
                break;
            }
            case 3: {
                clearConsole();
                std::cout << "\n+===========================================+\n";
                std::cout << "|              RESET PASSWORD               |\n";
                std::cout << "+===========================================+\n\n";
                
                users.showResetPasswordUI();
                pauseScreen();
                break;
            }
            case 0:
                std::cout << "\nTerima kasih telah menggunakan sistem.\n";
                return 0;
            default:
                std::cout << "\n[ERROR] Menu tidak valid. Silakan pilih 0-3.\n";
                pauseScreen();
        }
    }
}