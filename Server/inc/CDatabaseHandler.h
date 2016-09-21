#pragma once
#include "IDatabaseHandler.h"
#include <string.h>
#include <pthread.h>
#include <ITask.h>

class CDatabaseHandler: public IDatabaseHandler, public ITask
{
	private:
		pthread_mutex_t dbmtx;
		pthread_cond_t dbcond;
		int m_flag;
		std::string m_userLogin;
		std::string m_userPassword;

	public:
		CDatabaseHandler();
		CDatabaseHandler(int, std::string);
		CDatabaseHandler(int, std::string, std::string);
		~CDatabaseHandler();
		void createUser(std::string, std::string);
		void deleteUser(std::string, std::string);
		bool authenticate(std::string, std::string);
		bool findUser(std::string);
		bool isOnline(std::string);
		void run();
};

