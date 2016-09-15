#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <cstdio>
#include "MWorkQueue.h"
#include "MTask.h"
#define private public
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

	wq.m_finished=false;
	EXPECT_EQ(wq.addTask(&mt), 0);
	EXPECT_EQ(wq.tasks.size(), 1);	

}

int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
