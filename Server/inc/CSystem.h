#pragma once
#include "ISystem.h"

class CSystem: public ISystem
{
    private:

    public:
        CSystem();
        ~CSystem();
        int sockets(int, int, int);
        int binds(int, const struct sockaddr *, socklen_t);
        int listens(int, int);
        int accepts(int, struct sockaddr*, socklen_t*);
        int recvs(int, void*, size_t, int);
        int writes(int, const void*, size_t);

};
