#include <cstdio>
#include "ITask.h"

class IWorkQueue
{
public:
	virtual ~IWorkQueue(){};
	virtual ITask* nextTask()=0;
	virtual int addTask(ITask*)=0;
	virtual void finished()=0;
	virtual bool hasWork()=0;

};
