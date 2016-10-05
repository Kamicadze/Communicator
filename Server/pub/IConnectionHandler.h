#pragma once
#include <netinet/in.h>
#include <iostream>

#include "SFrame.h"

class IConnectionHandler
{
    public:
        virtual ~IConnectionHandler(){}
        virtual int listening()=0;
        virtual int clientHandler()=0;
        virtual int writeAnswer(std::string, int)=0;
        virtual int socketCreator()=0;
        virtual int handshake()=0;
        virtual int binding(sockaddr_in&)=0;
        virtual int accepting(int&, sockaddr_in&, int&)=0;
        virtual int recving(SFrame&, int&)=0;

};
