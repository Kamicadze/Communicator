#pragma once
#include "IDatabaseHandler.h"
#include "gmock/gmock.h"

class MDatabaseHandler: public IDatabaseHandler
{
    public:
        MDatabaseHandler(){};
        ~MDatabaseHandler(){};
        MOCK_METHOD1(findUser, bool(std::string));
        MOCK_METHOD2(createUser, bool(std::string, std::string));
        MOCK_METHOD2(deleteUser, bool(std::string, std::string));
        MOCK_METHOD2(authenticate, bool(std::string, std::string));
};
