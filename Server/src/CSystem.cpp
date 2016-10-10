#include "CSystem.h"
#include <unistd.h>

CSystem::CSystem()
{}

CSystem::~CSystem()
{}

int CSystem::sockets(int family, int sockType, int flag)
{
    int retValue;
    retValue=socket(family, sockType, flag);
    return retValue;
}

int CSystem::binds(int sock, const sockaddr *addr, socklen_t len)
{
    int retValue;
    retValue=bind(sock, addr, len);
    return retValue;
}

int CSystem::listens(int sock, int backlog)
{
    int retValue;
    retValue=listen(sock, backlog);
    return retValue;
}

int CSystem::accepts(int sock, sockaddr *addr, socklen_t *len)
{
    int retValue;
    retValue=accept(sock, addr, len);
    return retValue;
}

int CSystem::recvs(int sock, void *buff, size_t size, int flag)
{
    int retValue;
    retValue=recv(sock, buff, size, flag);
    return retValue;
}

int CSystem::writes(int sock, const void* buff, size_t size)
{
    int retValue;
    retValue=write(sock, buff, size);
    return retValue;
}

int CSystem::pthread_creates(pthread_t *thread , const pthread_attr_t *attr, void* (*start_rout) (void*), void *arg)
{
    int retValue;
    retValue=pthread_create(thread, attr, start_rout, arg);
    return retValue;
}

int CSystem::pthread_joins(pthread_t thread, void ** retval)
{
    int retValue;
    retValue=pthread_join(thread, retval);
    return retValue;
}
