#include "MWorkQueue.h"
#include "CThPool.h"
#include "ITask.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Return;

TEST(AddTaskTest, ThPool)
{
    MWorkQueue wq;
    ITask *it;
    CThPool pool(1);
    pool.m_oWorkQueue = wq;
    EXPECT_CALL(wq, addTask(_))
        .Times(2)
        .WillOnce(Return(1))
        .WillOnce(Return(0));
    EXPECT_CALL(wq, finished())
        .Times(1);
    EXPECT_EQ(pool.addTask(it), 1);
    pool.finish();
    EXPECT_EQ(pool.addTask(it), 0);
    

}
int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
