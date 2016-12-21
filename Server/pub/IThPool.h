#pragma once
#include "ITask.h"
class IThPool
{
    public:
    virtual ~IThPool(){}
    virtual int init(){}
    virtual int clear(){}
    virtual int addTask(ITask*)=0;
    virtual void finish()=0;
    virtual bool hasWork()=0;
    virtual void waitForCompletion()=0;
};

