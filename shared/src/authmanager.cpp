#include "authmanager.h"

extern ServerSocket* m_ServerSocket;

// Constructor

AuthManager::AuthManager(string keyFile) : 
    m_keyFile(keyFile)
{
    string key = GetKey();
    if (key == "")
        throw "Key is empty";
}

// private functions

string AuthManager::GetKey(){
    ifstream in(m_keyFile);
    if (!in.is_open())
        throw "Cannot open key file!";
    string key = "", line;
    while (getline(in, line)){
        key += line;
    }
    return key;
}

// public functions

void AuthManager::Handle(){
    vector<map<int, AuthSock>::iterator> del;
    for (auto it = m_auth.begin(); it != m_auth.end(); it++){
        if (m_ServerSocket->IsSocketExists(it->first))
            continue;
        del.push_back(it);
    }
    for (const auto& x : del){
        m_auth.erase(x);
    }
}

bool AuthManager::Auth(const int& sd, const string& key){
    time_t now = time(NULL);
    if (m_auth.count(sd) != 0){
        if (m_auth[sd].access)
            return false;
        // FIXME blokada czasu per IP
    }
    AuthSock as{sd, now, false};
    if (key == GetKey())
        as.access = true;
    m_auth[sd] = as;
    // syslog(LOG_DEBUG, "AUTH %s == %s %s", &key[0], &(GetKey())[0], &(m_ServerSocket->GetSocketInfo(sd))[0]);
    return as.access;
}
void AuthManager::Unauth(const int& sd){
    if (m_auth.count(sd) == 0)
        return;
    // FIXME czy nie powinienem sprawdzać czy access==true
    m_auth.erase(sd);
    syslog(LOG_INFO, "Deauthenticated %s", &(m_ServerSocket->GetSocketInfo(sd))[0]);
}
bool AuthManager::IsAuthenticated(const int& sd){
    if (m_auth.count(sd) == 0)
        return false;
    return m_auth[sd].access;
}