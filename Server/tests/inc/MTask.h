#include "gmock/gmock.h"
#include "ITask.h"

class MTask: public ITask
{
	public:
		MTask(){}
		~MTask(){}
		MOCK_METHOD0(run, void());
		MOCK_METHOD0(showTask, int());
};
