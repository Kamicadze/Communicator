#pragma once
#include "gmock/gmock.h"
#include "ISystem.h"

class MSystem: public ISystem
{
    public:
        MSystem(){}
        ~MSystem(){}
        MOCK_METHOD2(listens, int(int, int));
        MOCK_METHOD2(pthread_joins, int(pthread_t, void**));
        MOCK_METHOD3(sockets, int(int, int, int));
        MOCK_METHOD3(binds, int(int, const struct sockaddr*, socklen_t));
        MOCK_METHOD3(accepts, int(int, struct sockaddr*, socklen_t*));
        MOCK_METHOD3(writes, int(int, const void*, size_t));
        MOCK_METHOD4(recvs, int(int, void*, size_t, int));
        MOCK_METHOD4(pthread_creates, int(pthread_t*, const pthread_attr_t*, void* (*) (void*), void*));
};
