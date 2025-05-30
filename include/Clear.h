#pragma once

#include <cstdlib>
#include <iostream>

inline void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}