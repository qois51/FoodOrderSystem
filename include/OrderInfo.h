#pragma once
#include <string>
#include <unordered_map>
#include <ctime>
#include "Date.h"

using namespace std;

struct OrderInfo {
    Date tanggalPemesanan;
    string status;
    unordered_map<string, int> itemPesanan; 
    string Pelanggan;

    OrderInfo() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);

        tanggalPemesanan.year = now->tm_year + 1900;
        tanggalPemesanan.month = now->tm_mon + 1;
        tanggalPemesanan.day = now->tm_mday;

        string status = "diproses";
    };

    OrderInfo(const unordered_map<string, int>& items) : OrderInfo() {
        itemPesanan = items;
    } 
};