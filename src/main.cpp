#include <iostream>
#include <string>
#include "UserDB.h"
#include "OrdersDB.h"

using namespace std;

int main() {
    UserDB users("data/userData.csv");
    OrdersDB orders("data/orderData.csv");

    int choice;
    cout << "===== SISTEM PEMESANAN =====" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Lupa Password" << endl;
    cout << "Pilih menu (1/2/3): ";
    cin >> choice;
    cin.ignore(); // buang newline

    if (choice == 1) {
        string username, password;
        cout << "\n=== LOGIN ===" << endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        if (users.checkLogin(username, password)) {
            cout << "Login berhasil!\n";
            // Tambahkan fitur lanjutan setelah login di sini
        } else {
            cout << "Username atau password salah.\n";
        }

    } else if (choice == 2) {
        string username, nama, password, role;

        cout << "\n=== REGISTRASI ===" << endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Nama Lengkap: ";
        getline(cin, nama);
        cout << "Password: ";
        getline(cin, password);
        cout << "Role (pelanggan/petugas): ";
        getline(cin, role);

        if (users.addUser(username, nama, password, role)) {
            users.saveToFile();
            cout << "Akun berhasil dibuat!\n";
        } else {
            cout << "Username sudah digunakan. Registrasi gagal.\n";
        }

    } else if (choice == 3) {
        string username, nama, newPass;

        cout << "\n=== LUPA PASSWORD ===" << endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Nama Lengkap (autentikasi): ";
        getline(cin, nama);
        cout << "Password Baru: ";
        getline(cin, newPass);

        if (users.resetPassword(username, nama, newPass)) {
            users.saveToFile();
            cout << "Password berhasil diubah.\n";
        } else {
            cout << "Data tidak cocok. Password gagal diubah.\n";
        }

    } else {
        cout << "Menu tidak valid.\n";
    }

    return 0;
}