#pragma once
#include "ITask.h"
#include "IThPool.h"
#include "CWorkQueue.h"
#include <pthread.h>
#include <unistd.h>
#include <map>

class CThPool: public IThPool
{
private:
    pthread_t *m_threads;
    int m_numThreads;
    CWorkQueue m_oWorkQueue;

public:
    CThPool(int);
    ~CThPool();
    int init();
    int clear();
    void addTask(ITask*);
    void finish();
    bool hasWork();
    void waitForCompletion();
};
