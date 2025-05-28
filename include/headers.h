#pragma once
#ifndef HEADERS_H
#define HEADERS_H

#include <iostream>
#include <string>
#include <limits>
#include <chrono>
#include <thread>

inline void showMainHeader() {
    std::cout << "\n+=============================================+\n";
    std::cout << "||  >> FAST-FOOD ORDER MANAGEMENT SYSTEM <<  ||\n";
    std::cout << "+=============================================+\n\n";
}

inline void pauseScreen() {
    std::cout << "\nTekan Enter untuk melanjutkan...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

inline void delay(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

#endif