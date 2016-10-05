#pragma once
#include <sys/types.h>
#include <sys/socket.h>

class ISystem
{
    private:

    public:
        virtual ~ISystem(){}
        virtual int sockets(int, int, int)=0;
        virtual int binds(int, const struct sockaddr *, socklen_t )=0;
        virtual int listens(int, int)=0;
        virtual int accepts(int, struct sockaddr*, socklen_t*)=0;
        virtual int recvs(int, void*, size_t, int)=0;
        virtual int writes(int, const void*, size_t)=0;
};
