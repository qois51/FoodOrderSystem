#include <iostream>
#include "UserDB.h"
#include "OrdersDB.h"

int main() {
    OrdersDB orders("data/orderData.csv");
    UserDB users("data/userData.csv", &orders);

    int choice;
    cout << "===== SISTEM PEMESANAN =====" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Lupa Password" << endl;
    cout << "Pilih menu (1/2/3): ";
    cin >> choice;
    cin.ignore();

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
        default:
            cout << "Menu tidak valid\n";
    }

    return 0;
}