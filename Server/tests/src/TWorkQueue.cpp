#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>
#include "MWorkQueue.h"
#include "MTask.h"
#include "CWorkQueue.h"

using ::testing::AtLeast;
using ::testing::Return;
using ::testing::_;



TEST(FinishedFlagTest, FlagInterpretation)
{
	CWorkQueue wq;
	MTask mt;

	wq.m_finished=true;
	EXPECT_EQ(wq.addTask(&mt), -1);
	EXPECT_EQ(wq.hasWork(), false);

	wq.m_finished=false;
	EXPECT_EQ(wq.addTask(&mt), 0);
	EXPECT_EQ(wq.tasks.size(), 1);
	EXPECT_EQ(wq.hasWork(), true);	
	
	wq.finished();
	EXPECT_EQ(wq.m_finished, true);

}

TEST(TaskLifeTest, TaskLifeSpan)
{
	CWorkQueue wq;
	MTask mt;

	EXPECT_EQ(wq.addTask(&mt), 0);
	EXPECT_EQ(wq.tasks.size(), 1);
	EXPECT_EQ(wq.hasWork(), true);

	wq.nextTask();
	EXPECT_EQ(wq.tasks.size(), 0);
	EXPECT_EQ(wq.hasWork(), false);

}

int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
