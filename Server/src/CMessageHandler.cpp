#include "CMessageHandler.h"
#include "SFrame.h"
#include <sstream>
#include <string.h>
#include <sys/socket.h>

CMessageHandler::CMessageHandler()
{}

    CMessageHandler::CMessageHandler(ISystem* sys)
:m_sys(sys)
{}

CMessageHandler::~CMessageHandler()
{}

int CMessageHandler::recving(int sock, SFrame &frame)
{
    int result;
    fd_set readset;

    do {
        FD_ZERO(&readset);
        FD_SET(sock, &readset);
        result=select(sock +1, &readset, NULL, NULL, NULL);

    } while(-1 == result && EINTR==errno && false==endOfServerFlag);
    if(false==endOfServerFlag)
    {

        if(0<result)
        {
            if(FD_ISSET(sock, &readset))
            {
                result=m_sys->recvs(sock, &frame, sizeof(frame), MSG_WAITALL);
                if(0==result)
                {
                    return 2;
                }
                else
                {
                    return 0;
                }

            }
        }
        else if(result < 0)
        {
            std::cerr<<"ERROR: message not recived"<<std::endl;

            return 1;
        }
    }
    return 1;
}

int CMessageHandler::writing(int sock, const void* buff, size_t size)
{
    if(0>(m_sys->writes(sock, buff, size)))
    {  
        std::cerr<<"ERROR: message was not send"<<std::endl;
        return 1;
    }
    else
    {
        return 0;
    }

}

int CMessageHandler::sendToAll(SFrame frame)
{
    SFrame o_allFrame;
    o_allFrame=frame;
    int retVal=0;

    for(auto it=broad.begin(); it!=broad.end(); ++it)
    {
        if(it->first!=frame.m_CID)
        {

            strcpy(o_allFrame.m_DCID, it->first.c_str());
            o_allFrame.m_dataType=BROADCASTMSG;
            o_allFrame.m_destenationPort=it->second;
            //TODO: add a possibility to add write to task queue
            if(1==writing(it->second, &o_allFrame, sizeof(o_allFrame)))
            {
                ++retVal;
                std::string buff="Cannot write to user: ";
                buff.append(it->first);
                strcpy(frame.m_messageData, buff.c_str());
                int porto=broad[frame.m_CID];
                //TODO sending to host info bout error
                if(1==writing(porto, &frame, sizeof(frame)))
                {
                    ++retVal;
                    std::cerr<<"ERROR: writing to client in broadcast"<<std::endl;
                }
            }

        }

    }
    return retVal;
}

void CMessageHandler::broadcast(int port, std::string login)
{

    SFrame o_frame;
    broad[login]=port;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    while(false==endOfServerFlag)
    {
        recving(port, o_frame);
        dt=static_cast<int>(o_frame.m_dataType);
        if(dt==BROADCASTMSG)
        {
            sendToAll(o_frame);
        }
        else if(dt==EXITMSG)
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

int CMessageHandler::createChatRoom(SFrame cliFrame, int cliSock)
{
    int retVal=0;
    chatRooms[cliFrame.m_CID]=cliSock;  
    std::istringstream ss;
    ss.str(cliFrame.m_messageData);
    std::string login, nUsers;
    SFrame invFrame;
    nUsers=cliFrame.m_messageData;

    while(ss>>login)
    {
        if(online.find(login)!=online.end())
        {
            invFrame=cliFrame;
            strcpy(invFrame.m_DCID, login.c_str());
            invFrame.m_destenationPort=online[login];
            sprintf(invFrame.m_messageData, "Invite");
            invFrame.m_dataType=INVITATION; 
            if(1==writing(invFrame.m_destenationPort, &invFrame, sizeof(invFrame)))
            {
                ++retVal;
                std::string buff="Cannot write to user: ";
                invFrame.m_dataType=EXITMSG; 

                buff.append(login);
                strcpy(invFrame.m_messageData, buff.c_str());
                if(1==writing(cliSock, &invFrame, sizeof(invFrame)))
                {
                    ++retVal;
                    std::cerr<<"ERROR: writing to client in broadcast"<<std::endl;
                }
            }
        }
        else
        {
            std::string buff=login;

            buff.append(" is offline \n");
            invFrame.m_dataType=EXITMSG; 
            strcpy(invFrame.m_messageData, buff.c_str());
            writing(cliSock, &invFrame, sizeof(invFrame));
        }
    }

    hostHandler(cliFrame.m_CID, cliSock, nUsers);


    return retVal;
}

void CMessageHandler::inviteAccept(std::string login, std::string host, int port)
{
    SFrame frame;

    if((chatRooms.end()!=chatRooms.find(host))&&(online.end()!=online.find(host)))
    {
        // send information of accepting an invitation to host and get a map
        strcpy(frame.m_DCID, host.c_str());
        strcpy(frame.m_CID, login.c_str());
        frame.m_sourcePort=port;
        frame.m_destenationPort=online[host];
        sprintf(frame.m_messageData, "Invite accepted");
        frame.m_dataType=NEWUSERJOINED; 
        writing(frame.m_destenationPort, &frame, sizeof(frame));

        chatRoomHandler(login, port, host);


    }
    else
    {
        //TODO: send an information to user that the chat room is no longer enable
    }
}

void CMessageHandler::chatRoomHandler(std::string login, int port, std::string host)
{
    SFrame o_frame;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    std::string buff;

    std::map<std::string, int> chat;
    chat[login]=port;


    while(false==endOfServerFlag)
    {
        recving(port, o_frame);
        dt=static_cast<int>(o_frame.m_dataType);

        if(CHAT==dt)
        {
            writeToChat(chat, o_frame);
        }
        else if(EXITMSG==dt)//EXIT
        {
            o_frame.m_dataType=QUITINGUSER;
            strcpy(o_frame.m_CID, login.c_str());
            writing(port, &o_frame, sizeof(o_frame));
            for(auto it=chat.begin(); it!=chat.end(); ++it)
            {
                if(it->first!=login)
                {
                    o_frame.m_destenationPort=it->second;
                    //TODO: add a possibility to add write to task queue
                    writing(it->second, &o_frame, sizeof(o_frame));
                }
            }

            break;
        }

        else if(QUITINGUSER==dt)//other user quit
        {
            if(host==o_frame.m_CID)
            {
                o_frame.m_dataType=64;
                writing(port, &o_frame, sizeof(o_frame));
                break;
            }
            else
            {
                chat.erase(o_frame.m_CID);
            }
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

    SFrame o_frame;
    memset(&o_frame, 0, sizeof(o_frame));
    int dt=0;
    std::string nLogin, buff;


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

    while(false==endOfServerFlag)
    {
        recving(port, o_frame);
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

            writing(invitePort, &inviteFrame, sizeof(inviteFrame));

        }
        else if(CHAT==dt)
        {
            writeToChat(chat, o_frame);
        }
        else if(EXITMSG==dt)//EXIT
        {
            o_frame.m_dataType=QUITINGUSER;
            strcpy(o_frame.m_CID, login.c_str());
            writing(port, &o_frame, sizeof(o_frame));
            for(auto it=chat.begin(); it!=chat.end(); ++it)
            {
                if(it->first!=login)
                {
                    o_frame.m_destenationPort=it->second;
                    //TODO: add a possibility to add write to task queue
                    writing(it->second, &o_frame, sizeof(o_frame));
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
                    writing(it->second, &ansFrame, sizeof(ansFrame));
                }
            }

            ansFrame.m_dataType=USERSLIST;
            strcpy(ansFrame.m_messageData, chatList.c_str());
            writing(o_frame.m_sourcePort, &ansFrame, sizeof(ansFrame));


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
            writing(it->second, &chatFrame, sizeof(chatFrame));

        }

    }


}
