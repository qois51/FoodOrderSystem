
#include "auth.h"

bool cekLogin(const std::string& username, const std::string& password) {
    
    const std::string validUser = "admin";
    const std::string validPass = "1234";

    return (username == validUser && password == validPass);
}
