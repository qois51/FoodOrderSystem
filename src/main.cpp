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
        std::cout << " [1] Login\n";
        std::cout << " [2] Register\n";
        std::cout << " [3] Lupa Password\n";
        std::cout << " [0] Keluar\n";
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
                pauseScreen();
                break;
            }
            case 3: {
                clearConsole();

                users.showResetPasswordUI();
                break;
            }
            case 0:
                std::cout << "\nTerima kasih telah menggunakan sistem.\n";
                pauseScreen();
                return 0;
                break;
            default:
                std::cout << "\nPilihan tidak valid. Silakan coba lagi.\n";
                pauseScreen();
                break;
        }
    }
}