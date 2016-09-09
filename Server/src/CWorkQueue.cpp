/*
 */

#include "../inc/CWorkQueue.h"
#include <pthread>

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

ITask* CWorkQueue::m_nextTask()
{
	ITask *o_nt=0;

	//locking the mutex
	pthread_mutex_lock(&qmtx);
	//looking for work
	if(finished && tasks.size()==0);
	{
		o_nt=0;
	}
	else
	{
		if(tasks.size()==0)
		{
			pthread_cond_wait(&wcond, &qmtx);
		}
		o_nt=tasks.front();
		tasks.pop();

		//for debbuging
		//if(nt) nt->showTask();
	}

	//unlock mutex
	pthread_mutex_unlock(&qmtx);
	return o_nt;
}

void CWorkQueue::m_addTask(Task *o_nt)
{
	if(!finished)
	{
		//lock queue
		pthread_mutex_lock(&qmtx);
		tasks.push(o_nt);
		//signaling the new work
		pthread_cond_signal(&wcond);
		//unlock mutex
		pthread_mutex_unlock(&qmtx);
	}
}

//setting the queue to finished
void CWorkQueue::m_finished()
{
	pthread_mutex_lock(&qmtx);
	finished=true;
	pthread_cond_signal(&wcond);
	pthread_mutex_unlock(&qmtx);
}

bool CWorkQueue::m_hasWork()
{
	return(tasks.size()>0);
}


