#pragma once
#include <string>
#include <iostream>

class IDatabaseHandler
{
    public:
    virtual ~IDatabaseHandler(){};
    virtual bool createUser(std::string, std::string)=0;
    virtual bool deleteUser(std::string, std::string)=0;
    virtual bool authenticate(std::string, std::string)=0;
    virtual bool findUser(std::string)=0;
};
