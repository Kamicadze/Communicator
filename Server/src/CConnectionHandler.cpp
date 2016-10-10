#include "CConnectionHandler.h"
#include <unistd.h>
#include <sys/socket.h>
//#include <sys/select.h>
#include <netdb.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Globals.h"
#include <sys/time.h>
#include <sys/types.h>
#include "SFrame.h"
#include <sstream>
#include "CMessageHandler.h"
#include "CDatabaseHandler.h"
using namespace std;


CConnectionHandler::CConnectionHandler(int flag, IThPool *tp, int clisocket)
    :m_flag(flag),
    m_clisocket(clisocket),
    m_tp(tp)
{

}

CConnectionHandler::CConnectionHandler(int flag, IThPool *tp, ISystem *sys)
    :m_flag(flag),
    m_tp(tp),
    m_sys(sys)
{

}

CConnectionHandler::CConnectionHandler(int flag, int clisock, SFrame cliFrame, IDatabaseHandler *dbh)
    :m_flag(flag),
    m_clisocket(clisock),
    m_clientFrame(cliFrame),
    m_dbh(dbh)
    
{}

CConnectionHandler::~CConnectionHandler()
{}



void CConnectionHandler::run()
{
    switch(m_flag)
    {

        case SHAKING:
            handshake();
            break;

        case CLIENTHANDLING:
            clientHandler();
            break;

        default:
            break;
    }

}

int CConnectionHandler::socketCreator()
{
    m_socketfd=m_sys->sockets(AF_INET, SOCK_STREAM, 0);
    if(m_socketfd < 0)
    {
        std::cerr<<"ERROR: opening socket"<<std::endl;
        return UNSUCCESFUL;
    }
    else
    {
        return SUCCES;
    }
}

int CConnectionHandler::binding(sockaddr_in &serv_addr)
{
    if(m_sys->binds(m_socketfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr))<0)
    {
        std::cerr<<"ERROR: binding";
        return UNSUCCESFUL;
    }
    else
    {
        return SUCCES;
    }

}

int CConnectionHandler::listening()
{
    if(m_sys->listens(m_socketfd, 5)==-1)
    {
        std::cerr<<"ERROR: listening"<<std::endl;;
        return UNSUCCESFUL;
    }
    else
    {
        return SUCCES;
    }

}

int CConnectionHandler::accepting(int &newsockfd, sockaddr_in &cli_addr, int &clilen)
{
    newsockfd=m_sys->accepts(m_socketfd, reinterpret_cast<sockaddr *>(&cli_addr), (socklen_t*)&clilen);
    if(newsockfd < 0)
    {
        std::cerr<<"ERROR: accepting";
        return UNSUCCESFUL;
    }
    else
    {
        return SUCCES;
    }

}

int CConnectionHandler::recving(SFrame &cliFrame, int &newsockfd)
{
    if(0>(m_sys->recvs(newsockfd, &cliFrame, sizeof(cliFrame), MSG_WAITALL)))
    {  
        std::cerr<<"ERROR: message not recived"<<std::endl;
        return UNSUCCESFUL;
    }
    else
    {
        return SUCCES;
    }

}

int CConnectionHandler::handshake()
{
    int port, clilen, newsockfd, retValue;
    sockaddr_in serv_addr, cli_addr;
    SFrame cliFrame;
    CDatabaseHandler *o_dbh=new CDatabaseHandler();

    retValue=socketCreator();

    memset(&serv_addr, 0, sizeof(serv_addr));
    port=5001;

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port= htons(port);

    if(SUCCES==retValue)
    {
        retValue=binding(serv_addr);
    }


    while(false==endOfServerFlag && SUCCES==retValue)
    {
        retValue=listening();

        clilen = sizeof(cli_addr);

        if(0==retValue)
        {
            retValue=accepting(newsockfd, cli_addr, clilen);
        }


        if(false==endOfServerFlag && SUCCES==retValue)
        {
            if(SUCCES==recving(cliFrame, newsockfd))
            {
                CConnectionHandler *ch=new CConnectionHandler(CLIENTHANDLING, newsockfd, cliFrame, o_dbh);

                if(ch && o_dbh)
                {
                    m_tp->addTask(ch);
                }
                else
                {
                    delete ch;
                    delete o_dbh;
                }
            }
        }
        newsockfd=0;

        memset(&cliFrame ,0 ,sizeof(cliFrame));

    }
    close(newsockfd);
    close(m_socketfd);
    return retValue;


}
int CConnectionHandler::clientHandler()
{
    string buff;

    string tmp;
    int controlFlag=SUCCES;
    int dt=0;
    string login, password;
    CMessageHandler o_mh;
    while(false==endOfServerFlag)
    {
        istringstream ss;
        buff=reinterpret_cast<char*>(m_clientFrame.m_messageData);
        ss.str(buff);   
        dt=static_cast<int>(m_clientFrame.m_dataType);




        switch(dt)
        {

            case LOGGING:     ///handshake case
                login.clear();
                password.clear();
                ss >> login >> password;


                if((true==m_dbh->authenticate(login, password))&&(online.find(login)==online.end()))
                {
                    online[login]=m_clisocket;
                    tmp.clear();
                    tmp="Success";
                    writeAnswer(tmp, LOGGING);

                }
                else
                {
                    tmp.clear();
                    tmp="Wrong Login and/or Password";
                    writeAnswer(tmp, EXIT);

                }

                break;

            case DELETING:
                login.clear();
                password.clear();
                ss >> login >> password;


                if(true==m_dbh->authenticate(login, password))
                {
                    online.erase(login);
                    m_dbh->deleteUser(login, password);

                    tmp.clear();
                    tmp="Success";
                    writeAnswer(tmp, DELETING);

                }
                else
                {
                    tmp.clear();
                    tmp="Wrong Login and/or password";
                    writeAnswer(tmp, EXIT);
                }

                break;

            case BROADCAST:

                o_mh.broadcast(m_clisocket, m_clientFrame.m_CID);

                break;

            case CHATROOM:

                o_mh.createChatRoom(m_clientFrame, m_clisocket);
                break;

            case EXIT:     ///goodbye case
                online.erase(login);
                controlFlag=UNSUCCESFUL;
                break;

            case JOININGCHAT:
                //TODO: joining chat room
                o_mh.inviteAccept(m_clientFrame.m_CID, m_clientFrame.m_messageData, m_clisocket);
                break;


            case CREATINGUSER:
                ss >> login >> password;

                if(true==m_dbh->createUser(login, password))
                {
                    tmp.clear();
                    tmp="Succes";
                    writeAnswer(tmp, CREATINGUSER);

                }
                else
                {

                    tmp.clear();
                    tmp="Cannot add a user";
                    writeAnswer(tmp, EXIT);
                }

                break;

            case PASSONLINE: //map sender
                tmp.clear();
                for(auto it=online.begin(); it!=online.end(); it++)
                {
                    if((tmp.length()+it->first.length()+2)>149)
                    {
                        m_clientFrame.m_dataType=PASSONLINE;
                        sprintf(m_clientFrame.m_messageData, "%s", tmp.c_str());

                        write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
                        tmp.clear();
                    }
                    tmp.append(it->first);
                    tmp.append("  ");
                }
                m_clientFrame.m_dataType=PASSONLINE;
                sprintf(m_clientFrame.m_messageData, "%s", tmp.c_str());

                write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
                break;

            case SHUTDOWN:


            default:


                break;

        }
        if(UNSUCCESFUL==controlFlag)
        {
            break;
        }
        buff.clear();
        memset(&m_clientFrame ,0 ,sizeof(m_clientFrame) );
        //        rv=select(m_clisocket, &readfds, NULL, NULL, &tv);
        //        if(-1==rv)
        //        {
        //            std::cerr<<"Error: select"<<std::endl;
        //        }
        //        else if(0==rv)
        //        {
        //TODO: information to user about timeout
        //            std::cout<<"User timeout"<<std::endl;
        //           tmp.clear();
        //           tmp="You were not active for too long!";
        //            writeAnswer(tmp, 66);
        //           break;
        //        }
        //        else
        //        {
        if(false==endOfServerFlag)
        {
            recv(m_clisocket, &m_clientFrame, sizeof(m_clientFrame), MSG_WAITALL);
        }
        //        }
    }

    tmp.clear();
    tmp="Server Terminated!";
    writeAnswer(tmp, 66);
    std::cout<<"sent"<<std::endl;
    close(m_clisocket);
    std::cout<<"closed"<<std::endl;
    return 0;
}
int CConnectionHandler::writeAnswer(std::string msg, int dataType)
{
    uint32_t tmp32;
    uint8_t tmp8;
    istringstream ss;
    ss.str(string());   
    ss.str(m_clientFrame.m_CID);    
    ss >> m_clientFrame.m_DCID;
    sprintf(m_clientFrame.m_CID, "Server");
    tmp32=m_clientFrame.m_sourceAddress;
    m_clientFrame.m_sourceAddress=m_clientFrame.m_destenationAddress;
    m_clientFrame.m_destenationAddress=tmp32;

    tmp8=m_clientFrame.m_sourcePort;
    m_clientFrame.m_sourcePort=m_clientFrame.m_destenationPort;
    m_clientFrame.m_destenationPort=tmp8;

    m_clientFrame.m_dataType=dataType;

    strcpy(m_clientFrame.m_messageData, msg.c_str());

    write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));

    return 0;

}

