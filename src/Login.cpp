#include "Login.h"
#include <iostream>
#include <string>

bool login(UserDB& userDB) {
    std::string username;
    std::string password;

    std::cout << "Silahkan masukkan username: :";
    std::getline(std::cin, username);
    
    std::cout << "Silahkan masukkan password: ";
    std::getline(std::cin, password);

    if (userDB.checkLogin(username, password)) {
        std::cout << "Login berhasil!\n";
        return true;
    } else {
        std::cout << "Password atau username salah!\n";
        return false;
    }
}