#include <iostream>
// #include "hello.h"
#include "UserDB.h"
#include "OrdersDB.h"
#include "auth.h"

int main() {
    std::string username, password;

    std::cout << "=== Login Sistem Pemesanan ===\n";
    std::cout << "Username: ";
    std::cin >> username;
    std::cout << "Password: ";
    std::cin >> password;

    if (cekLogin(username, password)) {
        std::cout << "Login berhasil!\n";

        // Lanjutkan ke fitur lainnya setelah login
        // UserDB users("./data/userData.csv");
        OrdersDB orders("./data/orderData.csv");

        // Tambahkan logika berikutnya seperti tampilkan menu, input pesanan, dsb.

    } else {
        std::cout << "Username atau password salah.\n";
    }

    return 0;
}