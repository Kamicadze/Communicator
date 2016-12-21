#pragma once
#include "ITask.h"
#include "IThPool.h"
#include "CWorkQueue.h"
#include <pthread.h>
#include <unistd.h>
#include <map>
#include "CSystem.h"
#include "gmock/gmock.h"

class CThPool: public IThPool
{
private:
    pthread_t *m_threads;
    int m_numThreads;
    IWorkQueue *m_oWorkQueue;
    ISystem *m_sys;
    FRIEND_TEST(AddTaskTest, ThPool);

public:
    CThPool(int, IWorkQueue*, ISystem *);
    virtual ~CThPool();
    int init();
    int clear();
    int addTask(ITask*);
    void finish();
    bool hasWork();
    void waitForCompletion();
};
