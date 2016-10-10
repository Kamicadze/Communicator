#include "MWorkQueue.h"
#include "CThPool.h"
#include "ITask.h"
#include "MSystem.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::_;
using ::testing::Return;

TEST(AddTask, AddTaskTest)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(1, &wq, &sys);
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

TEST(InitTest, FirstThreadErrorTest)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(1)
        .WillOnce(Return(1));

    EXPECT_EQ(pool.init(), 1);
    
}

TEST(InitTest, MiddleThreadErrorTest)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(5)
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(1));

    EXPECT_EQ(pool.init(), 1);
    
}

TEST(InitTest, LastThreadErrorTest)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(10)
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(1));

    EXPECT_EQ(pool.init(), 1);
    
}

TEST(InitTest, InitGoesRightTest)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(10)
        .WillRepeatedly(Return(0));

    EXPECT_EQ(pool.init(), 0);
    
}

TEST(ClearTest, FirstThreadError)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(10)
        .WillRepeatedly(Return(0));

    EXPECT_CALL(sys, pthread_joins(_, _))
        .Times(1)
        .WillOnce(Return(1));
    
    EXPECT_CALL(wq, finished())
        .Times(1);

    EXPECT_CALL(wq, hasWork())
        .Times(1)
        .WillOnce(Return(false));

    EXPECT_EQ(pool.init(), 0);
    EXPECT_EQ(pool.clear(), 1);

}

TEST(ClearTest, MiddleThreadError)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(10)
        .WillRepeatedly(Return(0));

    EXPECT_CALL(sys, pthread_joins(_, _))
        .Times(5)
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(1));
    
    EXPECT_CALL(wq, finished())
        .Times(1);

    EXPECT_CALL(wq, hasWork())
        .Times(1)
        .WillOnce(Return(false));

    EXPECT_EQ(pool.init(), 0);
    EXPECT_EQ(pool.clear(), 1);

}

TEST(ClearTest, LastThreadError)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(10)
        .WillRepeatedly(Return(0));

    EXPECT_CALL(sys, pthread_joins(_, _))
        .Times(10)
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(1));
    
    EXPECT_CALL(wq, finished())
        .Times(1);

    EXPECT_CALL(wq, hasWork())
        .Times(1)
        .WillOnce(Return(false));

    EXPECT_EQ(pool.init(), 0);
    EXPECT_EQ(pool.clear(), 1);

}

TEST(ClearTest, AllGoesRight)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(sys, pthread_creates(_, _, _, _))
        .Times(10)
        .WillRepeatedly(Return(0));

    EXPECT_CALL(sys, pthread_joins(_, _))
        .Times(10)
        .WillRepeatedly(Return(0));
    
    EXPECT_CALL(wq, finished())
        .Times(1);

    EXPECT_CALL(wq, hasWork())
        .Times(1)
        .WillOnce(Return(false));

    EXPECT_EQ(pool.init(), 0);
    EXPECT_EQ(pool.clear(), 0);

}

TEST(HasWorkTest, HasWork)
{
    MWorkQueue wq;
    ITask *it;
    MSystem sys;
    CThPool pool(10, &wq, &sys);

    EXPECT_CALL(wq, hasWork())
        .Times(2)
        .WillOnce(Return(false))
        .WillOnce(Return(true));


    EXPECT_EQ(pool.hasWork(), false);
    EXPECT_EQ(pool.hasWork(), true);

}


int main(int argc, char** argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
