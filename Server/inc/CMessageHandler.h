#pragma once
#include <map>
#include <iostream>
#include <string.h>
#include "SFrame.h"
#include "Globals.h"
#include "CThPool.h"
#include "CSystem.h"

class CMessageHandler
{
    private:
        std::map<std::string, int> m_chatUsers;
        std::string m_host;
        ISystem *m_sys;

    public:
        CMessageHandler();
        CMessageHandler(ISystem*);
        ~CMessageHandler();
        int createChatRoom(SFrame, int);
        void broadcast(int, std::string);
        int sendToAll(SFrame);
        void chatRoomHandler(std::string, int, std::string);
        void writeToChat(std::map<std::string, int> &, SFrame);
        void inviteAccept(std::string, std::string, int);
        void hostHandler(std::string, int, std::string);
        int writing(int, const void*, size_t);
        int recving(int sock, SFrame&);

};

enum EChatHandlers
{
    BROADCASTMSG=3,
    CHAT=4,
    EXITMSG=5,
    INVITATION=6,
    NEWINVITATION=42,
    QUITINGUSER=45,
    NEWUSERJOINED=46,
    USERSLIST=47


};
