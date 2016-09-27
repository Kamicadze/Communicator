#pragma once
#include <map>
#include <iostream>
#include <string.h>
#include "SFrame.h"
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
    void createChatRoom(SFrame, int, CThPool*);
    void broadcast(CThPool*, int, std::string);
    void sendToAll(CThPool*, SFrame);
    void chatRoomHandler(std::string, int, std::string, CThPool*);
    void writeToChat(std::map<std::string, int>, SFrame);


};
