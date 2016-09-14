#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../inc/MWorkQueue.h"
#include <cstdio>
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;



TEST(AddTaskTest, AddingTaskwalkthrought)
{
	testing::NiceMock<MWorkQueue> que;
	int i=10;
	EXPECT_CALL(que, addTask((ITask*)i))
		.Times(AtLeast(1))
		.WillOnce(Return(0));

	que.addTask((ITask*)i);

	EXPECT_TRUE(que.hasWork()==0);

	EXPECT_CALL(que, nextTask());

	que.nextTask();

	EXPECT_CALL(que, finished());
	que.finished();

	EXPECT_CALL(que, addTask((ITask*)i))
		.WillOnce(Return(-1));
	que.addTask((ITask*)i);

		

}

int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
