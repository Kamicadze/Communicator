#include "CMessageHandler.h"
#include "SFrame.h"
#include <sstream>
#include <string.h>
#include <sys/socket.h>

CMessageHandler::CMessageHandler()
{}

CMessageHandler::~CMessageHandler()
{}

enum CMessageHandler::ChatHandlers_t: int
{
    BROADCAST=3,
    CHAT=4,
    EXIT=5,
    INVITATION=6,
    NEWINVITATION=42,
    QUITINGUSER=45,
    NEWUSERJOINED=46,
    USERSLIST=47

    
};

void CMessageHandler::sendToAll(SFrame frame)
{
    SFrame o_allFrame;
    for(auto it=broad.begin(); it!=broad.end(); ++it)
    {
        if(it->first!=frame.m_CID)
        {

            o_allFrame=frame;
            strcpy(o_allFrame.m_DCID, it->first.c_str());
            o_allFrame.m_dataType=BROADCAST;
            o_allFrame.m_destenationPort=it->second;
            //TODO: add a possibility to add write to task queue
            write(it->second, &o_allFrame, sizeof(o_allFrame));

        }

    }
}

void CMessageHandler::broadcast(int port, std::string login)
{

    SFrame o_frame;
    broad[login]=port;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    while(1)
    {
        recv(port, &o_frame, sizeof(o_frame), MSG_WAITALL);
        dt=static_cast<int>(o_frame.m_dataType);
        if(dt==BROADCAST)
        {
            sendToAll(o_frame);
        }
        else if(dt==EXIT)
        {
            break;
        }
        else
        {
            //TODO:error handling
        }
        memset(&o_frame, 0, sizeof(o_frame));
    }
    broad.erase(login); 

}

void CMessageHandler::createChatRoom(SFrame cliFrame, int cliSock)
{
    chatRooms[cliFrame.m_CID]=cliSock;  
    std::istringstream ss;
    ss.str(cliFrame.m_messageData);
    std::string login, nUsers;
    SFrame invFrame;
    nUsers=cliFrame.m_messageData;

    while(ss>>login)
    {
        invFrame=cliFrame;
        strcpy(invFrame.m_DCID, login.c_str());
        invFrame.m_destenationPort=online[login];
        sprintf(invFrame.m_messageData, "Invite");
        invFrame.m_dataType=INVITATION; ///6 is invite type for user and join chat for server
        write(invFrame.m_destenationPort, &invFrame, sizeof(invFrame));
    }

    hostHandler(cliFrame.m_CID, cliSock, nUsers);



}

void CMessageHandler::inviteAccept(std::string login, std::string host, int port)
{
    SFrame frame;

    if((chatRooms.end()!=chatRooms.find(host))&&(online.end()!=online.find(host)))
    {
        //TODO: send information of accepting an invitation to host and get a map
        strcpy(frame.m_DCID, host.c_str());
        strcpy(frame.m_CID, login.c_str());
        frame.m_sourcePort=port;
        frame.m_destenationPort=online[host];
        sprintf(frame.m_messageData, "Invite accepted");
        frame.m_dataType=NEWUSERJOINED; 
        write(frame.m_destenationPort, &frame, sizeof(frame));
        //chat room data types:
        //6- invite for user
        //4- standart message
        //5- exit
        //45- erase
        //42- reserved for invites(comming soon TM)
        //46- invite accept
        chatRoomHandler(login, port, host);


    }
    else
    {
        //TODO: send an information to user that the chat room is no longer enable
    }
}
//TODO: user only chat handler and seprate host only handler
//host: -recives inv accept and sends reply with users list -whenever new user comes online he sends the rest information -when he exits sends info to all that they exit too
//user: -gets his map -sends to all that he quits -do map staff
//
void CMessageHandler::chatRoomHandler(std::string login, int port, std::string host)
{
    SFrame o_frame;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    std::string buff;

    std::map<std::string, int> chat;
    chat[login]=port;


    while(1)
    {
        recv(port, &o_frame, sizeof(o_frame), MSG_WAITALL);
        dt=static_cast<int>(o_frame.m_dataType);

        if(CHAT==dt)
        {
            writeToChat(chat, o_frame);
        }
        else if(EXIT==dt)//EXIT
        {
            o_frame.m_dataType=QUITINGUSER;
            strcpy(o_frame.m_CID, login.c_str());
            write(port, &o_frame, sizeof(o_frame));
            for(auto it=chat.begin(); it!=chat.end(); ++it)
            {
                if(it->first!=login)
                {
                    o_frame.m_destenationPort=it->second;
                    //TODO: add a possibility to add write to task queue
                    write(it->second, &o_frame, sizeof(o_frame));
                }
            }

            break;
        }

        else if(QUITINGUSER==dt)//other user quit
        {
            chat.erase(o_frame.m_CID);
        }
        else if(NEWUSERJOINED==dt)//someone accepted an invite
        {
            chat[o_frame.m_messageData]=online[o_frame.m_messageData];
        }
        else if(USERSLIST==dt)//for new user data
        {
            std::string currentUsr=o_frame.m_messageData;

            std::stringstream chatUsers;
            chatUsers << currentUsr;
            currentUsr.clear();
            while(chatUsers >> currentUsr)
            {
                chat[currentUsr]=online[currentUsr];
            }
        }

        memset(&o_frame, 0, sizeof(o_frame));
    }

}

void CMessageHandler::hostHandler(std::string login, int port, std::string invs)
{

    //TODO:: start is host only, host have to send other list of users once again, or find other way to do this
    //other way: in cli frame send host name as DCID and inv list i data


    SFrame o_frame;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    std::string nLogin, buff;
    int nPort;

    std::map<std::string, int> chat;
    chat[login]=port;
    std::stringstream ss;
    ss << invs;
    while(ss >> nLogin)
    {
        if(nLogin!=login )
        {
            chat[nLogin]=0;
        }
    }

    while(1)
    {
        recv(port, &o_frame, sizeof(o_frame), MSG_WAITALL);
        dt=static_cast<int>(o_frame.m_dataType);

        if(NEWINVITATION==dt)
        {
            std::string inviteLogin;
            inviteLogin=o_frame.m_messageData;
            int invitePort=online[inviteLogin];
            SFrame inviteFrame;
            memset(&inviteFrame, 0, sizeof(inviteFrame));
            sprintf(inviteFrame.m_CID, "%s",  login.c_str());
            sprintf(inviteFrame.m_DCID, "%s", inviteLogin.c_str());
            inviteFrame.m_dataType=INVITATION;
            inviteFrame.m_sourcePort=port;
            inviteFrame.m_destenationPort=invitePort;
            sprintf(inviteFrame.m_messageData, "Invite");

            write(invitePort, &inviteFrame, sizeof(inviteFrame));

        }
        else if(CHAT==dt)
        {
            writeToChat(chat, o_frame);
        }
        else if(EXIT==dt)//EXIT
        {
            o_frame.m_dataType=QUITINGUSER;
            strcpy(o_frame.m_CID, login.c_str());
            write(port, &o_frame, sizeof(o_frame));
            for(auto it=chat.begin(); it!=chat.end(); ++it)
            {
                if(it->first!=login)
                {
                    o_frame.m_destenationPort=it->second;
                    //TODO: add a possibility to add write to task queue
                    write(it->second, &o_frame, sizeof(o_frame));
                }
            }
            chatRooms.erase(login);
            break;
        }

        else if(QUITINGUSER==dt)//other user quit
        {
            chat.erase(o_frame.m_CID);
        }
        else if(NEWUSERJOINED==dt)//someone accepted an invite
        {

            SFrame ansFrame;
            std::string chatList =login;
            memset(&ansFrame, 0, sizeof(ansFrame));
            sprintf(ansFrame.m_CID, "%s",  login.c_str());
            ansFrame.m_dataType=NEWUSERJOINED;
            ansFrame.m_sourcePort=port;
            strcpy(ansFrame.m_messageData, o_frame.m_CID);

            for(auto it=chat.begin(); it!=chat.end(); ++it)
            {

                if(!((0==login.compare(it->first)) || (0==it->second)) )
                {
                    chatList += "  ";
                    chatList += it->first;
                    strcpy(ansFrame.m_DCID, it->first.c_str());
                    ansFrame.m_destenationPort=it->second;
                    write(it->second, &ansFrame, sizeof(ansFrame));
                }
            }

            ansFrame.m_dataType=USERSLIST;
            strcpy(ansFrame.m_messageData, chatList.c_str());
            write(o_frame.m_sourcePort, &ansFrame, sizeof(ansFrame));


            chat[o_frame.m_CID]=online[o_frame.m_CID];
        }

        memset(&o_frame, 0, sizeof(o_frame));
    }

}
void CMessageHandler::writeToChat(std::map<std::string, int> &chat, SFrame frame)
{

    SFrame chatFrame;
    for(auto it=chat.begin(); it!=chat.end(); ++it)
    {
        if(it->first!=frame.m_CID && 0<it->second)
        {

            chatFrame=frame;
            strcpy(chatFrame.m_DCID, it->first.c_str());
            chatFrame.m_dataType=CHAT;
            chatFrame.m_destenationPort=it->second;
            //TODO: add a possibility to add write to task queue
            write(it->second, &chatFrame, sizeof(chatFrame));

        }

    }


}
