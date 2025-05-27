#pragma once
#ifndef HEADERS_H
#define HEADERS_H

#include <iostream>
#include <string>

inline void showMainHeader() {
    std::cout << "\n+=============================================+\n";
    std::cout << "||  >> FAST-FOOD ORDER MANAGEMENT SYSTEM <<  ||\n";
    std::cout << "+=============================================+\n\n";
}

inline void pauseScreen() {
    std::cout << "\nTekan Enter untuk melanjutkan...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif