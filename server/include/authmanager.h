#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include "authsock.h"

#include <map>
#include <string>

using namespace std;

class AuthManager{
private:
    map<int, AuthSock> m_auth;
public:
    AuthManager();

    void Handle();

    void Auth(const int&, const string&);
    void Unauth(const int&);
    bool IsAuthenticated(const int&);
};

#endif