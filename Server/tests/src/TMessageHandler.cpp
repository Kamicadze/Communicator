#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "MSystem.h"
#include "CMessageHandler.h"
#include "SFrame.h"
#include "Globals.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::DoAll;
using ::testing::Invoke;

TEST(SendToAllTest, DstWriteFail)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;
    broad["kami"]=2;

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(2)
        .WillOnce(Return(-1))
        .WillOnce(Return(1));

    EXPECT_EQ(1, mh.sendToAll(frame));
    broad.erase("kami");
}

TEST(SendToAllTest, DstAndSrcWriteFail)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;
    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 
    broad["kami"]=2;

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(2)
        .WillOnce(Return(-1))
        .WillOnce(Return(-1));

    EXPECT_EQ(2, mh.sendToAll(frame));

}

TEST(SendToAllTest, DstWriteSucc)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 
    broad["kami"]=2;

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(1)
        .WillOnce(Return(0));

    EXPECT_EQ(0, mh.sendToAll(frame));

}

TEST(SendToAllTest, DstWriteFailMidFor)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;
    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 
    broad["kami"]=2;
    broad["tami"]=3;
    broad["sami"]=1;

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(4)
        .WillOnce(Return(1))
        .WillOnce(Return(-1))
        .WillOnce(Return(1))
        .WillOnce(Return(1));

    EXPECT_EQ(1, mh.sendToAll(frame));

}

TEST(SendToAllTest, DstAndSrcWriteFailMidFor)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 
    broad["kami"]=2;
    broad["tami"]=3;
    broad["sami"]=1;

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(4)
        .WillOnce(Return(1))
        .WillOnce(Return(-1))
        .WillOnce(Return(-1))
        .WillOnce(Return(1));

    EXPECT_EQ(2, mh.sendToAll(frame));

}

TEST(SendToAllTest, DstAndSrcWriteFailMidForX2)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 
    broad["kami"]=2;
    broad["tami"]=3;
    broad["sami"]=1;

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(5)
        .WillOnce(Return(1))
        .WillOnce(Return(-1))
        .WillOnce(Return(-1))
        .WillOnce(Return(-1))
        .WillOnce(Return(-1));

    EXPECT_EQ(4, mh.sendToAll(frame));

}

TEST(SendToAllTest, DstWriteFailMidForX2)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 
    broad["kami"]=2;
    broad["tami"]=3;
    broad["sami"]=1;

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(5)
        .WillOnce(Return(1))
        .WillOnce(Return(-1))
        .WillOnce(Return(1))
        .WillOnce(Return(-1))
        .WillOnce(Return(1));

    EXPECT_EQ(2, mh.sendToAll(frame));

}

ACTION_P(SetToFrame, value)
{
    *static_cast<SFrame*>(arg1)=value;
}

TEST(BroadcastTest, RecvDT3AndDT5)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;
    frame.m_dataType=3;
    strcpy(frame.m_messageData, "cos");
    strcpy(frame.m_CID, "kami");
    SFrame frame2;
    frame2.m_dataType=5;
    strcpy(frame2.m_messageData, "cos");
    strcpy(frame2.m_CID, "kami");



//    ON_CALL(sys, recvs(_, _, _, _))
//        .WillByDefault(DoAll(SetToFrame(frame2), Return(1)));

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 

    broad["kami"]=2;

    EXPECT_CALL(sys, recvs(_, _,_,_))
        .Times(2)
        .WillOnce(DoAll(SetToFrame(frame), Return(1)))
        .WillOnce(DoAll(SetToFrame(frame2), Return(1)));

    EXPECT_CALL(sys, writes(_,_,_))
        .Times(1)
        .WillOnce(Return(1));

    mh.broadcast(3, "tami");

}

TEST(CreatChatRoomTest, InviteFail)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;
    strcpy(frame.m_messageData, "kami");
    strcpy(frame.m_CID, "tami");
    SFrame frame2;
    frame2.m_dataType=5;
    online["tami"]=3;

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 


    EXPECT_CALL(sys, writes(_,_,_))
        .Times(4)
        .WillOnce(Return(-1))
        .WillOnce(Return(1))
        .WillOnce(Return(1))
        .WillOnce(Return(1));

    EXPECT_CALL(sys, recvs(_, _,_,_))
        .Times(1)
        .WillOnce(DoAll(SetToFrame(frame2), Return(1)));

    EXPECT_EQ(1, mh.createChatRoom(frame, 2));

}

TEST(CreatChatRoomTest, InviteAndWriteFail)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;
    strcpy(frame.m_messageData, "kami");
    strcpy(frame.m_CID, "tami");
    SFrame frame2;
    frame2.m_dataType=5;
    online["tami"]=3;

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 


    EXPECT_CALL(sys, writes(_,_,_))
        .Times(4)
        .WillOnce(Return(-1))
        .WillOnce(Return(-1))
        .WillOnce(Return(1))
        .WillOnce(Return(1));

    EXPECT_CALL(sys, recvs(_, _,_,_))
        .Times(1)
        .WillOnce(DoAll(SetToFrame(frame2), Return(1)));

    EXPECT_EQ(2, mh.createChatRoom(frame, 2));

}

TEST(CreatChatRoomTest, InviteSucces)
{
    MSystem sys;
    CMessageHandler mh(&sys);
    SFrame frame;
    strcpy(frame.m_messageData, "kami");
    strcpy(frame.m_CID, "tami");
    SFrame frame2;
    frame2.m_dataType=5;
    online["tami"]=3;

    for(auto it=broad.begin(); it!=broad.end(); it++)
    {
        broad.erase(it->first);        
    } 


    EXPECT_CALL(sys, writes(_,_,_))
        .Times(3)
        .WillOnce(Return(1))
        .WillOnce(Return(1))
        .WillOnce(Return(1));

    EXPECT_CALL(sys, recvs(_, _,_,_))
        .Times(1)
        .WillOnce(DoAll(SetToFrame(frame2), Return(1)));

    EXPECT_EQ(0, mh.createChatRoom(frame, 2));

}



int main(int argc, char** argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
