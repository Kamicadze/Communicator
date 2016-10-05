#pragma once
#include <map>
#include <iostream>
#include <string.h>
#include "SFrame.h"
#include "Globals.h"
#include "CThPool.h"

class CMessageHandler
{
    private:
    std::map<std::string, int> m_chatUsers;
    std::string m_host;

    public:
    CMessageHandler();
    CMessageHandler(std::string);
    ~CMessageHandler();
    void createChatRoom(SFrame, int);
    void broadcast(int, std::string);
    void sendToAll(SFrame);
    void chatRoomHandler(std::string, int, std::string);
    void writeToChat(std::map<std::string, int> &, SFrame);
    void inviteAccept(std::string, std::string, int);
    void hostHandler(std::string, int, std::string);


};
