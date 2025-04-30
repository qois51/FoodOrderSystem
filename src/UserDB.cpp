#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "UserInfo.h"
#include "UserDB.h"

using namespace std;

UserDB::UserDB(string filelocation) {
    string line;
    ifstream file(filelocation);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << filelocation << endl;
        return;
    }

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string username, nama, pass, role;

        getline(ss, username, ',');
        getline(ss, nama, ',');
        getline(ss, pass, ',');
        getline(ss, role, ',');

        userMap[username] = UserInfo(nama, pass, role);
    }

    cout << userMap["qois51"].nama << endl;
}