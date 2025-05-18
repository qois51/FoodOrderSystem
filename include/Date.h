#pragma once

#include <string>
#include <sstream>
#include <iomanip>

struct Date {
    int year;
    int month;
    int day;
};

inline Date getCurrentDate() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    Date currentDate;
    currentDate.year = now->tm_year + 1900;
    currentDate.month = now->tm_mon + 1;
    currentDate.day = now->tm_mday;

    return currentDate;
}

inline std::string dateToString(const Date& date) {
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << date.year << "-"
        << std::setw(2) << std::setfill('0') << date.month << "-"
        << std::setw(2) << std::setfill('0') << date.day;
    return oss.str();
}