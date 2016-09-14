/*
 */
#include <cstdio>
#include <iostream>
#include "CWorkQueue.h"
#include "Globals.h"

CWorkQueue::CWorkQueue()
{
	//initialization of mutex to protect the queue
	pthread_mutex_init(&qmtx,0);

	//wcond signals when new work is available
	pthread_cond_init(&wcond,0);

}

CWorkQueue::~CWorkQueue()
{
	pthread_mutex_destroy(&qmtx);
	pthread_cond_destroy(&wcond);
}

ITask* CWorkQueue::nextTask()
{
	ITask *o_nt=0;

	//locking the mutex
	pthread_mutex_lock(&qmtx);
	//looking for work
	if(m_finished && 0==tasks.size())
	{
		o_nt=0;
	}
	else
	{
		if(0==tasks.size())
		{
			std::cout<<"i'm w8ing threa:"<<pthread_self()<<std::endl;
			pthread_cond_wait(&wcond, &qmtx);
		}
		o_nt=tasks.front();
		tasks.pop();

		//for debbuging
	//	if(o_nt) o_nt->showTask();
	}

	//unlock mutex
	pthread_mutex_unlock(&qmtx);
	return o_nt;
}

int CWorkQueue::addTask(ITask *o_nt)
{
	int return_value=-1;
	if(!m_finished)
	{
		//lock queue
		pthread_mutex_lock(&qmtx);
		tasks.push(o_nt);
		//signaling the new work
		pthread_cond_signal(&wcond);
		//unlock mutex
		pthread_mutex_unlock(&qmtx);
		return_value=0;
	}
	else
	{
		return_value=-1;	
	}
	return return_value;
}

//setting the queue to finished
void CWorkQueue::finished()
{
	pthread_mutex_lock(&qmtx);
	m_finished=true;
	pthread_cond_signal(&wcond);
	pthread_mutex_unlock(&qmtx);
}

bool CWorkQueue::hasWork()
{
	bool return_value=false;
	if(0==tasks.size())
	{
		return_value=false;
	}
	else
	{
		return_value=true;
	}
	return return_value;
}


