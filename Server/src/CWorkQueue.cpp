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

ITask* CWorkQueue::nextTask()
{
	ITask *o_nt=0;
