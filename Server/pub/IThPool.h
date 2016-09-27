#pragma once
#include "ITask.h"
class IThPool
{
    public:
    virtual ~IThPool(){}
    virtual void addTask(ITask*)=0;
    virtual void finish()=0;
    virtual bool hasWork()=0;
    virtual void waitForCompletion()=0;
};

