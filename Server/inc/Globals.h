#pragma once
#include "ITask.h"
#include "CWorkQueue.h"
#include <pthread.h>
#include <iostream>
#include <cstdio>
#include <list>

//std::list<std::string> usersOnline;

static pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;

//void showTask(int n)
//{
//	pthread_mutex_lock(&console_mutex);
//	printf("task number: %d \n", n);
//	pthread_mutex_unlock(&console_mutex);
//}

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
