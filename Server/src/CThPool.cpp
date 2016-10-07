///This class takes care of creating threads and then joining them.
///Additionaly this class wraps methods of CWorkQueue class to use them next
///with threads in mind
///
#include "CThPool.h"
#include "Globals.h"
#include <iostream>
using namespace std;

CThPool::CThPool(int n)
    :m_threads(0),
    m_numThreads(n){
        m_threads = new pthread_t[n];
    for(int i=0;i<n;++i)
    {
        //creating new thread with function getWork which than
        //send it to waiting for a real task
        pthread_create(&(m_threads[i]),0, getWork, &m_oWorkQueue);
    }
    }

CThPool::~CThPool()
{
    m_oWorkQueue.finished();
    waitForCompletion();
    for(int i=0;i<m_numThreads;++i)
    {
        pthread_join(m_threads[i], 0);
    }
    delete[] m_threads;
}

int CThPool::init()
{

    return 0;
}

int CThPool::clear()
{


    return 0;
}

void CThPool::addTask(ITask *nt)
{

    m_oWorkQueue.addTask(nt);
}

void CThPool::finish()
{
    m_oWorkQueue.finished();
}

bool CThPool::hasWork()
{
    return m_oWorkQueue.hasWork();
}

void CThPool::waitForCompletion()
{

    while(m_oWorkQueue.hasWork())
    {}
}
