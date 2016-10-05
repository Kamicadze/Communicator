#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MSystem.h"
#include "CConnectionHandler.h"
#include "MThPool.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Ge;
using ::testing::Ne;

TEST(AllGoesWriteTest, Handshaking)
{
    MSystem sys;
    MThPool tp;
 
//    {
    
//    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(0));

/*    EXPECT_CALL(sys, binds(_, _, _))
        .Times(1)
        .WillOnce(Return(Ge(0)));

    EXPECT_CALL(sys, listens(_, _))
        .Times(1)
        .WillOnce(Return(Ne(-1)));

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(1)
        .WillOnce(Return(Ne(-1)));

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(1)
        .WillOnce(Return(Ne(-1)));*/
//    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();
}

int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
