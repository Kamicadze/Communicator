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


CConnectionHandler::CConnectionHandler(int flag, CThPool *tp, int clisocket)
    :m_flag(flag),
    m_clisocket(clisocket),
    m_tp(tp)
{

}

CConnectionHandler::CConnectionHandler(int flag, CThPool *tp)
    :m_flag(flag),
    m_tp(tp)
{

}

CConnectionHandler::CConnectionHandler(int flag, CThPool *tp, int clisock, SFrame cliFrame)
    :m_flag(flag),
    m_clisocket(clisock),
    m_tp(tp),
    m_clientFrame(cliFrame)
{}

CConnectionHandler::~CConnectionHandler()
{}

void CConnectionHandler::run()
{
    switch(m_flag)
    {

        case 2:
            listening();
            break;

        case 3:
            clientHandler();
            break;

        default:
            break;
    }

}

void CConnectionHandler::listening()
{
    int port, clilen, newsockfd, n;
    m_socketfd=socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr, cli_addr;
    SFrame cliFrame;



    if(m_socketfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    port=5001;

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_port= htons(port);

    if(bind(m_socketfd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr))<0)
    {
        perror("ERROR on binding");
        exit(1);
    }



    while(false==endOfServerFlag)
    {
        if(listen(m_socketfd, 5)==-1)
        {
            perror("listen");
            exit(1);
        }
        clilen = sizeof(cli_addr);

        newsockfd=accept(m_socketfd, reinterpret_cast<sockaddr *>(&cli_addr), (socklen_t*)&clilen);
        if(newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }


        if(false==endOfServerFlag)
        {
            
            n=recv(newsockfd, &cliFrame, sizeof(cliFrame), MSG_WAITALL);
            if(n<0)
            {
                //TODO: error handling
                std::cerr<<"Error: message not recived"<<std::endl;
            }

            CConnectionHandler *ch=new CConnectionHandler(3, m_tp, newsockfd, cliFrame);

            if(ch)
            {
                m_tp->addTask(ch);
            }
        }
        newsockfd=0;

        memset(&cliFrame ,0 ,sizeof(cliFrame));

    }
    close(newsockfd);
    close(m_socketfd);


}
void CConnectionHandler::clientHandler()
{
    string buff;

    string tmp;
    int controlFlag=0;
    int dt=0;
    string login, password;
    CDatabaseHandler o_dbh;
    CMessageHandler o_mh;
    while(false==endOfServerFlag)
    {
        istringstream ss;
        buff=reinterpret_cast<char*>(m_clientFrame.m_messageData);
        ss.str(buff);   
        dt=static_cast<int>(m_clientFrame.m_dataType);




        switch(dt)
        {

            case 1:     ///handshake case
                login.clear();
                password.clear();
                ss >> login >> password;


                if((true==o_dbh.authenticate(login, password))&&(m_tp->online.find(login)==m_tp->online.end()))
                {
                    m_tp->online[login]=m_clisocket;
                    tmp.clear();
                    tmp="Success";
                    writeAnswer(tmp, 1);

                }
                else
                {
                    tmp.clear();
                    tmp="Wrong Login and/or Password";
                    writeAnswer(tmp, 5);

                }

                break;

            case 2:
                login.clear();
                password.clear();
                ss >> login >> password;


                if(true==o_dbh.authenticate(login, password))
                {
                    m_tp->online.erase(login);
                    o_dbh.deleteUser(login, password);

                    tmp.clear();
                    tmp="Success";
                    writeAnswer(tmp, 2);

                }
                else
                {
                    tmp.clear();
                    tmp="Wrong Login and/or password";
                    writeAnswer(tmp, 5);
                }

                break;

            case 3:

                o_mh.broadcast(m_tp, m_clisocket, m_clientFrame.m_CID);

                break;

            case 4:

                o_mh.createChatRoom(m_clientFrame, m_clisocket, m_tp);
                break;

            case 5:     ///goodbye case
                m_tp->online.erase(login);
                controlFlag=1;
                break;

            case 6:
                //TODO: joining chat room
                o_mh.inviteAccept(m_clientFrame.m_CID, m_clientFrame.m_messageData, m_tp, m_clisocket);
                break;


            case 7:
                ss >> login >> password;

                if(true==o_dbh.createUser(login, password))
                {
                    tmp.clear();
                    tmp="Succes";
                    writeAnswer(tmp, 7);

                }
                else
                {

                    tmp.clear();
                    tmp="Cannot add a user";
                    writeAnswer(tmp, 5);
                }

                break;

            case 8: //map sender
                tmp.clear();
                for(auto it=m_tp->online.begin(); it!=m_tp->online.end(); it++)
                {
                    if((tmp.length()+it->first.length()+2)>149)
                    {
                        m_clientFrame.m_dataType=8;
                        sprintf(m_clientFrame.m_messageData, "%s", tmp.c_str());

                        write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
                        tmp.clear();
                    }
                    tmp.append(it->first);
                    tmp.append("  ");
                }
                m_clientFrame.m_dataType=8;
                sprintf(m_clientFrame.m_messageData, "%s", tmp.c_str());

                write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
                break;

            case 66:


            default:

                /*  ss.str(string());   
                    ss.str(m_clientFrame.m_CID);    
                    ss >> m_clientFrame.m_DCID;
                    sprintf(m_clientFrame.m_CID, "Server");
                    tmp32=m_clientFrame.m_sourceAddress;
                    m_clientFrame.m_sourceAddress=m_clientFrame.m_destenationAddress;
                    m_clientFrame.m_destenationAddress=tmp32;

                    tmp8=m_clientFrame.m_sourcePort;
                    m_clientFrame.m_sourcePort=m_clientFrame.m_destenationPort;
                    m_clientFrame.m_destenationPort=tmp8;

                    sprintf(m_clientFrame.m_messageData, "ERROR");

                    write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));*/
                break;

        }
        if(1==controlFlag)
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
        recv(m_clisocket, &m_clientFrame, sizeof(m_clientFrame), MSG_WAITALL);
        //        }
    }

    tmp.clear();
    tmp="Server Terminated!";
    writeAnswer(tmp, 66);
    std::cout<<"sent"<<std::endl;
    close(m_clisocket);
    std::cout<<"closed"<<std::endl;
}
void CConnectionHandler::writeAnswer(std::string msg, int dataType)
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

}

