#include "CMessageHandler.h"
#include "SFrame.h"
#include <sstream>
#include <string.h>
#include <sys/socket.h>

CMessageHandler::CMessageHandler()
{}

CMessageHandler::~CMessageHandler()
{}

void CMessageHandler::sendToAll(CThPool *tp, SFrame frame)
{
    SFrame o_allFrame;
    for(auto it=tp->broad.begin(); it!=tp->broad.end(); ++it)
    {
        if(it->first!=frame.m_CID)
        {
        
            o_allFrame=frame;
            strcpy(o_allFrame.m_DCID, it->first.c_str());
            o_allFrame.m_dataType=3;
            o_allFrame.m_destenationPort=it->second;
            //TODO: add a possibility to add write to task queue
            write(it->second, &o_allFrame, sizeof(o_allFrame));

        }
        
    }
}

void CMessageHandler::broadcast(CThPool *tp, int port, std::string login)
{

    SFrame o_frame;
    tp->broad[login]=port;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    while(1)
    {
        recv(port, &o_frame, sizeof(o_frame), MSG_WAITALL);
        dt=static_cast<int>(o_frame.m_dataType);
        if(dt==3)
        {
            sendToAll(tp, o_frame);
        }
        else if(dt==5)
        {
            break;
        }
        else
        {
            //TODO:error handling
        }
        memset(&o_frame, 0, sizeof(o_frame));
    }
    tp->broad.erase(login); 

}

void CMessageHandler::createChatRoom(SFrame cliFrame, int cliSock, CThPool *tp)
{
    tp->chatRooms[cliFrame.m_CID]=cliSock;  
    std::istringstream ss;
    ss.str(cliFrame.m_messageData);
    std::string login;
    SFrame invFrame;

    while(1)
    {
        ss >> login;
        if(login.length()>0)
        {
            invFrame=cliFrame;
            strcpy(invFrame.m_DCID, login.c_str());
            invFrame.m_destenationPort=tp->online[login];
            sprintf(invFrame.m_messageData, "Invite");
            invFrame.m_dataType=6; ///6 is invite type for user and join chat for server
        }
        else
        {
            break;
        }

    }

    chatRoomHandler(cliFrame.m_CID, cliSock, cliFrame.m_CID, tp);
    


}

void CMessageHandler::chatRoomHandler(std::string login, int port, std::string host, CThPool *tp)
{
    SFrame o_frame;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=-1;

    std::map<std::string, int> chat;
    chat[login]=port;
    if(host!=login)
    {
        SFrame newUserFrame;
        sprintf(newUserFrame.m_CID, "%s", login.c_str());
        sprintf(newUserFrame.m_DCID, "%s", host.c_str());
        newUserFrame.m_dataType=7; ///data type 7 for acceptin an invite
        newUserFrame.m_destenationPort=tp->chatRooms[host];
        sprintf(newUserFrame.m_messageData, "%s  %d ", login.c_str(), port);
        write(tp->chatRooms[host], &newUserFrame, sizeof(newUserFrame));



    }
    while(1)
    {
        recv(port, &o_frame, sizeof(o_frame), MSG_WAITALL);
        dt=static_cast<int>(o_frame.m_dataType);

        if(dt==2)
        {
            std::string inviteLogin;
            inviteLogin=o_frame.m_messageData;
            int invitePort=tp->online[inviteLogin];
            SFrame inviteFrame;
            memset(&inviteFrame, 0, sizeof(inviteFrame));
            sprintf(inviteFrame.m_CID, "%s",  host.c_str());
            sprintf(inviteFrame.m_DCID, "%s", inviteLogin.c_str());
            inviteFrame.m_dataType=6;
            inviteFrame.m_sourcePort=port;
            inviteFrame.m_destenationPort=invitePort;
            sprintf(inviteFrame.m_messageData, "Invite");

            write(invitePort, &inviteFrame, sizeof(inviteFrame));
            
        }
        else if(dt==4)
        {
            writeToChat(chat, o_frame);
        }
        else if(dt==5)
        {
            break;
        }
        else if(dt==7)
        {
        
        }
        memset(&o_frame, 0, sizeof(o_frame));
    }

}
void CMessageHandler::writeToChat(std::map<std::string, int> chat, SFrame frame)
{

    SFrame chatFrame;
    for(auto it=chat.begin(); it!=chat.end(); ++it)
    {
        if(it->first!=frame.m_CID)
        {
        
            chatFrame=frame;
            strcpy(chatFrame.m_DCID, it->first.c_str());
            chatFrame.m_dataType=4;
            chatFrame.m_destenationPort=it->second;
            //TODO: add a possibility to add write to task queue
            write(it->second, &chatFrame, sizeof(chatFrame));

        }
        
    }


}
