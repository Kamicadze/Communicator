///This class takes care of creating threads and then joining them.
///Additionaly this class wraps methods of CWorkQueue class to use them next
///with threads in mind
///
#include "CThPool.h"
#include "Globals.h"
#include <iostream>
using namespace std;

CThPool::CThPool(int n, IWorkQueue *wq, ISystem *sys)
    :m_threads(0),
    m_numThreads(n),
    m_oWorkQueue(wq),
    m_sys(sys)
{}

CThPool::~CThPool()
{}

int CThPool::init()
{
    int retVal=0;
    m_threads = new pthread_t[m_numThreads];
   
    for(int i=0;i<m_numThreads;++i)
    {
        //creating new thread with function getWork which than
        //send it to waiting for a real task



        if(0!=m_sys->pthread_creates(&(m_threads[i]),0, getWork, m_oWorkQueue))
        {
            std::cerr<<"ERROR: cannot create thread"<<std::endl;
            retVal=1;
            break;
        }
    }


    return retVal;
}

int CThPool::clear()
{
    int retVal=0;
    m_oWorkQueue->finished();
    waitForCompletion();
    for(int i=0;i<m_numThreads;++i)
    {
        if(0!=m_sys->pthread_joins(m_threads[i], 0))
        {
            std::cerr<<"ERROR: cannot end thread"<<std::endl;
            retVal=1;
            break;
        }
    }
    delete[] m_threads;

    return retVal;
}

int CThPool::addTask(ITask *nt)
{
    int retVal;
    retVal=m_oWorkQueue->addTask(nt);
    return retVal;
}

void CThPool::finish()
{
    m_oWorkQueue->finished();
}

bool CThPool::hasWork()
{
    return m_oWorkQueue->hasWork();
}

void CThPool::waitForCompletion()
{

    while(m_oWorkQueue->hasWork())
    {}
}
