#include "CThPool.h"
#include "Globals.h"
#include <iostream>
using namespace std;

CThPool::CThPool(int n)
	:m_numThreads(n){
		cout<<"Creating thpool with"<<n<<"threads"<<endl;
		m_threads = new pthread_t[n];
		for(int i=0;i<n;++i)
		{
			pthread_create(&(m_threads[i]),0, getWork, &o_workQueue);
		}
	}

CThPool::~CThPool()
{
	o_workQueue.finished();
	waitForCompletion();
	cout<<"after wFC"<<endl;
	for(int i=0;i<m_numThreads;++i)
	{
		cout<<"before join"<<endl;
		pthread_join(m_threads[i], 0);
		cout<<"Thread: "<<i+1<<"has ended his life"<<endl;
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
