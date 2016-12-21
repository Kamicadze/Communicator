#pragma once
#include "IThPool.h"
#include "gmock/gmock.h"



class MThPool: public IThPool
{
	public:
		MThPool(){}
		~MThPool(){}
		MOCK_METHOD0(finish, void());
		MOCK_METHOD0(hasWork, bool());
		MOCK_METHOD0(waitForCompletion, void());
		MOCK_METHOD1(addTask, void(ITask*));
};
