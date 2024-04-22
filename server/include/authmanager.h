#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include "authsock.h"

#include <map>
#include <string>
#include <fstream>

using namespace std;

class AuthManager{
private:
    map<int, AuthSock> m_auth;
    string m_keyFile;
    // const time_t AUTH_LOCK = 60;

    string GetKey();
public:
    AuthManager(string);

    void Handle();

    void Auth(const int&, const string&);
    void Unauth(const int&);
    bool IsAuthenticated(const int&);
};

#endif