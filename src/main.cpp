#include <iostream>
#include "UserDB.h"
#include "OrdersDB.h"
#include "Clear.h"

int main() {
    OrdersDB orders("data/orderData.csv");
    UserDB users("data/userData.csv", &orders);

    while (true) {
        clearConsole();
        int choice;
        std::cout << "===== SISTEM PEMESANAN =====" << std::endl;
        std::cout << "1. Login" << std::endl;
        std::cout << "2. Register" << std::endl;
        std::cout << "3. Lupa Password" << std::endl;
        std::cout << "0. Keluar" << std::endl;
        std::cout << "Pilih menu (0/1/2/3): ";
        std::cin >> choice;
        std::cin.ignore();

        switch(choice) {
            case 1: {
                std::string username = users.login();
                if (!username.empty()) {
                    std::cout << "Selamat datang, " << username << "!\n";
                    users.displayUserActivities(username);
                } else {
                    std::cout << "Gagal login, silakan coba lagi.\n";
                }
                break;
            }
            case 2:
                users.showRegistrationUI();
                break;
            case 3:
                users.showResetPasswordUI();
                break;
            case 0:
                std::cout << "Terima kasih telah menggunakan sistem.\n";
                return 0;
            default:
                std::cout << "Menu tidak valid\n";
        }
        std::cout << std::endl;
    }
}