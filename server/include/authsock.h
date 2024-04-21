#ifndef AUTH_SOCK_H
#define AUTH_SOCK_H

#include <ctime>

struct AuthSock{
    int fd;
    time_t timestamp;
};

#endif