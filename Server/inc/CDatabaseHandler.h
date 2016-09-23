#pragma once
#include "IDatabaseHandler.h"
#include <string.h>
#include <pthread.h>
#include <ITask.h>

class CDatabaseHandler: public IDatabaseHandler
{
	private:
		pthread_mutex_t dbmtx;
		pthread_cond_t dbcond;

	public:
		CDatabaseHandler();
		~CDatabaseHandler();
		void createUser(std::string, std::string);
		void deleteUser(std::string, std::string);
		bool authenticate(std::string, std::string);
		bool findUser(std::string);
};

