#pragma once
#include "ITask.h"
#include "CWorkQueue.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <map>

//static pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;

extern bool endOfServerFlag;

extern std::map<std::string, int> online;
extern std::map<std::string, int> chatRooms;
extern std::map<std::string, int> broad;

static void *getWork(void* par)
{
    ITask *mw=0;

    CWorkQueue *wq=(CWorkQueue*)par;
    while(mw=wq->nextTask())
    {
        mw->run();
        delete mw;
    }
    return 0;
}

