#pragma once
#include <string>
#include <unordered_map>
#include "userInfo.h"

using namespace std;

class UserDB {
private:
    unordered_map<string, UserInfo> userMap;
public:
    UserDB(string filelocation);
};