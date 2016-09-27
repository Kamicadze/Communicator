#pragma once
#include "IDatabaseHandler.h"
#include <string.h>
#include <pthread.h>
#include <ITask.h>

class CDatabaseHandler: public IDatabaseHandler
{
    private:
        pthread_mutex_t m_dbmtx;
        pthread_cond_t m_dbcond;

    public:
        CDatabaseHandler();
        ~CDatabaseHandler();
        bool createUser(std::string, std::string);
        bool deleteUser(std::string, std::string);
        bool authenticate(std::string, std::string);
        bool findUser(std::string);
};

