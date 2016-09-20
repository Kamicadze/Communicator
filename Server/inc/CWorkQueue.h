/*! \brief Header for CWorkQueue class. It's basic purpos is to wrap stl queue
 * to add some mutex protection
 *
 */

#pragma once
#include "gtest/gtest_prod.h"
#include "ITask.h"
#include "IWorkQueue.h"
#include <cstdio>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//It is a basic queue for tasks with some mutex protecting it
class CWorkQueue
{
	std::queue<ITask*>tasks;
//	std::queue<std::function<bool(void*)>> que;
	bool m_finished;
	pthread_mutex_t qmtx;
	pthread_cond_t wcond;
	FRIEND_TEST(FinishedFlagTest, FlagInterpretation);
	FRIEND_TEST(TaskLifeTest, TaskLifeSpan);

public:
	CWorkQueue();	
	~CWorkQueue();
	//gets task from queue
	ITask *nextTask();
	//adds new task to the queue
	int addTask(ITask*);
//	int addTask(bool (*f)(void*));
	//sets task queue as finished
	void finished();
	//checks if there is a work
	bool hasWork();



};

