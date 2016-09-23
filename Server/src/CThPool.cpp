///This class takes care of creating threads and then joining them.
///Additionaly this class wraps methods of CWorkQueue class to use them next
///with threads in mind
///
#include "CThPool.h"
#include "Globals.h"
#include <iostream>
using namespace std;

CThPool::CThPool(int n)
	:m_numThreads(n){
//		online["server"]=5001;
		m_threads = new pthread_t[n];
		for(int i=0;i<n;++i)
		{
			///creating new thread with function getWork which than
			///send it to waiting for a real task
			pthread_create(&(m_threads[i]),0, getWork, &o_workQueue);
		}
	}

CThPool::~CThPool()
{
	o_workQueue.finished();
	waitForCompletion();
	for(int i=0;i<m_numThreads;++i)
	{
		pthread_join(m_threads[i], 0);
	}
	delete[] m_threads;
	
}

void CThPool::addTask(ITask *nt)
{
	o_workQueue.addTask(nt);
}

void CThPool::finish()
{
	o_workQueue.finished();
}

bool CThPool::hasWork()
{
	return o_workQueue.hasWork();
}

void CThPool::waitForCompletion()
{
	
	while(true)
	{
		if(false==o_workQueue.hasWork())
		{
			break;
		}
	}
}
