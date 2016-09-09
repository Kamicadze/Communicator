/*! \brief Header for CWorkQueue class. It's basic purpos is to wrap stl queue
 * to add some mutex protection
 *
 */

#pragma once

#include "../pub/ITask.h"
#include <cstdio>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//It is a basic queue for tasks with some mutex protecting it
class CWorkQueue
{
	std::queue<ITask*>tasks;
	bool finished;
	pthread_mutex_t qmtx;
	pthread_cond_t wcond;

public:
	CWorkQueue();	
	~CWorkQueue();
	//gets task from queue
	ITask *m_nextTask();
	//adds new task to the queue
	void m_addTask(ITask*);
	//sets task queue as finished
	void m_finished();
	//checks if there is a work
	bool m_hasWork();



};

