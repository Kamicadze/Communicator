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
    void addTask(ITask*);
    void finish();
    bool hasWork();
    void waitForCompletion();

    std::map<std::string, int> online;
    std::map<std::string, int> broad;
    std::map<std::string, int> chatRooms; // metody na mapy
};
