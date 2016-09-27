#pragma once
#include <string>
#include "SFrame.h"

class CConnectionHandler
{
    private:

    public:
        CConnectionHandler();
        ~CConnectionHandler();
        int handshake(int, char*);
        void listening(int);
        SFrame frameCreator(int, std::string, std::string, int);
};
