#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MSystem.h"
#include "CConnectionHandler.h"
#include "MThPool.h"
#include "Globals.h"

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::Ge;
using ::testing::Ne;

TEST(SocketGoesWrong, Handshaking)
{
    MSystem sys;
    MThPool tp;
 
    {
    
    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(-1))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, binds(_, _, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, listens(_, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(0)
        .RetiresOnSaturation();
    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();
}

TEST(BindGoesWrong, Handshaking)
{
    MSystem sys;
    MThPool tp;
 
    {
    
    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, binds(_, _, _))
        .Times(1)
        .WillOnce(Return(-1))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, listens(_, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(0)
        .RetiresOnSaturation();
    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();
}

TEST(ListenGoesWrong, Handshaking)
{
    MSystem sys;
    MThPool tp;
 
    {
    
    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, binds(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, listens(_, _))
        .Times(1)
        .WillOnce(Return(-1))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(0)
        .RetiresOnSaturation();
    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();
}

TEST(AcceptGoesWrong, Handshaking)
{
    MSystem sys;
    MThPool tp;
 
    {
    
    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, binds(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, listens(_, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(1)
        .WillOnce(Return(-1))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(0)
        .RetiresOnSaturation();
    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();
}
/*
TEST(RecvGoesWrong, Handshaking)
{
    MSystem sys;
    MThPool tp;
 
    {
    
    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, binds(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, listens(_, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(1)
        .WillOnce(Return(-1))
        .RetiresOnSaturation();
    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();
}*/

TEST(FlagGoesTrue, Handshaking)
{
    MSystem sys;
    MThPool tp;

    endOfServerFlag=true;
 
    {
    
    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, binds(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, listens(_, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(0)
        .RetiresOnSaturation();

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(0)
        .RetiresOnSaturation();
    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();

    endOfServerFlag=false;
}

ACTION(FlagChange)
{
    endOfServerFlag=true;   
}
/*
TEST(FlagGoesTrueWhileListen, Handshaking)
{
    MSystem sys;
    MThPool tp;
 
    ON_CALL(sys,listens(_, _))
        .WillByDefault(FlagChange());

    {
    
    InSequence stdCall;

    EXPECT_CALL(sys, sockets(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, binds(_, _, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, listens(_, _))
        .Times(1)
        .WillOnce(Return(0))
        .RetiresOnSaturation();

    EXPECT_CALL(sys, accepts(_, _, _))
        .Times(1)
        .WillOnce(Return(-1))

        .RetiresOnSaturation();

    EXPECT_CALL(sys, recvs(_, _, _, _))
        .Times(0)
        .RetiresOnSaturation();
    }
    CConnectionHandler *ch=new CConnectionHandler(2, &tp, &sys);

    ch->run();
    std::cout<<"flag: "<<endOfServerFlag<<std::endl;
}*/

int main(int argc, char** argv)
{
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
