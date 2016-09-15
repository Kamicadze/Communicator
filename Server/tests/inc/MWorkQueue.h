#include "gmock/gmock.h"
#include "IWorkQueue.h"
#pragma once

class MWorkQueue: public IWorkQueue
{
	public:
		MWorkQueue(){}
		~MWorkQueue(){}
		MOCK_METHOD0(nextTask, ITask*());
		MOCK_METHOD0(finished, void());
		MOCK_METHOD0(hasWork, bool());
		MOCK_METHOD1(addTask, int(ITask*));
};
