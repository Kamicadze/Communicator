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
    std::string login, nUsers;
    SFrame invFrame;
    nUsers=cliFrame.m_messageData;

    while(ss>>login)
    {
        invFrame=cliFrame;
        strcpy(invFrame.m_DCID, login.c_str());
        invFrame.m_destenationPort=tp->online[login];
        sprintf(invFrame.m_messageData, "Invite");
        invFrame.m_dataType=6; ///6 is invite type for user and join chat for server
        write(invFrame.m_destenationPort, &invFrame, sizeof(invFrame));
    }

    hostHandler(cliFrame.m_CID, cliSock, tp, nUsers);



}

void CMessageHandler::inviteAccept(std::string login, std::string host, CThPool *tp, int port)
{
    SFrame frame;

    if((tp->chatRooms.end()!=tp->chatRooms.find(host))&&(tp->online.end()!=tp->online.find(host)))
    {
        //TODO: send information of accepting an invitation to host and get a map
        strcpy(frame.m_DCID, host.c_str());
        strcpy(frame.m_CID, login.c_str());
        frame.m_sourcePort=port;
        frame.m_destenationPort=tp->online[host];
        sprintf(frame.m_messageData, "Invite accepted");
        frame.m_dataType=46; 
        write(frame.m_destenationPort, &frame, sizeof(frame));
        //chat room data types:
        //6- invite for user
        //4- standart message
        //5- exit
        //45- erase
        //42- reserved for invites(comming soon TM)
        //46- invite accept
        chatRoomHandler(login, port, host, tp);


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
void CMessageHandler::chatRoomHandler(std::string login, int port, std::string host, CThPool *tp)
{
    SFrame o_frame;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    std::string nLogin, buff;
    int nPort;

    std::map<std::string, int> chat;
    chat[login]=port;


    while(1)
    {
        recv(port, &o_frame, sizeof(o_frame), MSG_WAITALL);
        dt=static_cast<int>(o_frame.m_dataType);

        if(4==dt)
        {
            writeToChat(chat, o_frame);
        }
        else if(5==dt)//EXIT
        {
            o_frame.m_dataType=45;
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

        else if(45==dt)//other user quit
        {
            chat.erase(o_frame.m_CID);
        }
        else if(46==dt)//someone accepted an invite
        {
            chat[o_frame.m_messageData]=tp->online[o_frame.m_messageData];
        }
        else if(47==dt)//for new user data
        {
            std::string currentUsr=o_frame.m_messageData;

            std::stringstream chatUsers;
            chatUsers << currentUsr;
            currentUsr.clear();
            while(chatUsers >> currentUsr)
            {
                chat[currentUsr]=tp->online[currentUsr];
            }
        }

        memset(&o_frame, 0, sizeof(o_frame));
    }

}

void CMessageHandler::hostHandler(std::string login, int port, CThPool *tp, std::string invs)
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

        if(42==dt)
        {
            std::string inviteLogin;
            inviteLogin=o_frame.m_messageData;
            int invitePort=tp->online[inviteLogin];
            SFrame inviteFrame;
            memset(&inviteFrame, 0, sizeof(inviteFrame));
            sprintf(inviteFrame.m_CID, "%s",  login.c_str());
            sprintf(inviteFrame.m_DCID, "%s", inviteLogin.c_str());
            inviteFrame.m_dataType=6;
            inviteFrame.m_sourcePort=port;
            inviteFrame.m_destenationPort=invitePort;
            sprintf(inviteFrame.m_messageData, "Invite");

            write(invitePort, &inviteFrame, sizeof(inviteFrame));

        }
        else if(4==dt)
        {
            writeToChat(chat, o_frame);
        }
        else if(5==dt)//EXIT
        {
            o_frame.m_dataType=45;
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
            tp->chatRooms.erase(login);
            break;
        }

        else if(45==dt)//other user quit
        {
            chat.erase(o_frame.m_CID);
        }
        else if(46==dt)//someone accepted an invite
        {

            SFrame ansFrame;
            std::string chatList =login;
            memset(&ansFrame, 0, sizeof(ansFrame));
            sprintf(ansFrame.m_CID, "%s",  login.c_str());
            ansFrame.m_dataType=46;
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

            ansFrame.m_dataType=47;
            strcpy(ansFrame.m_messageData, chatList.c_str());
            write(o_frame.m_sourcePort, &ansFrame, sizeof(ansFrame));


            chat[o_frame.m_CID]=tp->online[o_frame.m_CID];
        }

        memset(&o_frame, 0, sizeof(o_frame));
    }

}
void CMessageHandler::writeToChat(std::map<std::string, int> chat, SFrame frame)
{

    SFrame chatFrame;
    for(auto it=chat.begin(); it!=chat.end(); ++it)
    {
        if(it->first!=frame.m_CID && 0<it->second)
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
