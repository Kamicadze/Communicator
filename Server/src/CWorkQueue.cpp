///
///
///
 
#include <cstdio>
#include <iostream>
#include "CWorkQueue.h"
CWorkQueue::CWorkQueue()
{
    ///initialization of mutex to protect the queue
    pthread_mutex_init(&m_qmtx,0);

    ///wcond signals when new work is available
    pthread_cond_init(&m_wcond,0);

}

CWorkQueue::~CWorkQueue()
{
    pthread_mutex_destroy(&m_qmtx);
    pthread_cond_destroy(&m_wcond);
}

ITask* CWorkQueue::nextTask()
{
    ITask *o_nt=0;

    ///locking the mutex
    pthread_mutex_lock(&m_qmtx);
    ///looking for work
    if(m_finished && 0==tasks.size())
    {
        o_nt=0;
    }
    else
    {
        if(0==tasks.size())
        {
            pthread_cond_wait(&m_wcond, &m_qmtx);
        }
        if(!tasks.empty())
        {
            o_nt=tasks.front();
            tasks.pop();
        }
        else
        {
            o_nt=0;
        }
    }
    ///unlock mutex
    pthread_mutex_unlock(&m_qmtx);
    return o_nt;
}

int CWorkQueue::addTask(ITask *o_nt)
{
    int return_value=-1;
    if(!m_finished)
    {
        ///lock queue
        pthread_mutex_lock(&m_qmtx);
        tasks.push(o_nt);
        ///signaling the new work
        pthread_cond_signal(&m_wcond);
        ///unlock mutex
        pthread_mutex_unlock(&m_qmtx);
        return_value=0;
    }
    else
    {
        return_value=-1;    
    }
    return return_value;
}
///setting the queue to finished
void CWorkQueue::finished()
{
    pthread_mutex_lock(&m_qmtx);
    m_finished=true;
    pthread_cond_broadcast(&m_wcond);
    pthread_mutex_unlock(&m_qmtx);
}

bool CWorkQueue::hasWork()
{
    return (tasks.size()>0);
}

