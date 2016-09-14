#include "ITask.h"
#include "IThPool.h"
#include "CWorkQueue.h"
#include <pthread.h>
#include <unistd.h>


class CThPool: public IThPool
{
private:
	pthread_t *m_threads;
	int m_numThreads;
	CWorkQueue o_workQueue;

public:
	CThPool(int);
	~CThPool();
	void addTask(ITask*);
	void finish();
	bool hasWork();
	void waitForCompletion();
};
