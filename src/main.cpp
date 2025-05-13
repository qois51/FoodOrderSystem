#include <iostream>
#include "UserDB.h"

int main() {
    UserDB users("data/userData.csv");

    int choice;
    cout << "===== SISTEM PEMESANAN =====" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Lupa Password" << endl;
    cout << "Pilih menu (1/2/3): ";
    cin >> choice;
    cin.ignore();

    switch(choice) {
        case 1:
            if (users.login()) {
                std::cout << "Selamat datang!\n";
            } else {
                std::cout << "Gagal login, silakan coba lagi.\n";
            }
            break;
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