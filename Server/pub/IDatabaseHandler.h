#pragma once
#include <string>
#include <iostream>

class IDatabaseHandler
{
	public:
	virtual ~IDatabaseHandler(){};
	virtual void createUser(std::string, std::string)=0;
	virtual void deleteUser(std::string, std::string)=0;
	virtual bool authenticate(std::string, std::string)=0;
	virtual bool findUser(std::string)=0;
	virtual bool isOnline(std::string)=0;
};
