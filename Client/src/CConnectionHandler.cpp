#include "CMenu.h"
#include "CConnectionHandler.h"
#include <iostream>
#include "SFrame.h"
#include "Globals.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>


CConnectionHandler::CConnectionHandler()
{}

CConnectionHandler::~CConnectionHandler()
{}

int CConnectionHandler::handshake(int portNo, char* host)
{
    int sockfd;
    sockaddr_in servAddr;
    hostent *server;
    SFrame cliFrame;

    memset(&cliFrame, 0, sizeof(cliFrame));

    sockfd=socket(AF_INET, SOCK_STREAM,0);

    if(sockfd<0)
    {
        std::cerr<<"ERROR: opening socket"<<std::endl;
        return 1;
    }

    server = gethostbyname(host);

    if(server==NULL)
    {
        std::cerr<<"ERROR: no such host"<<std::endl;
        return 1;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family=AF_INET;

    memcpy(server->h_addr, &servAddr.sin_addr.s_addr, server->h_length);
    servAddr.sin_port=htons(portNo);

    if(connect(sockfd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr))<0)
    {
        std::cerr<<"Error: cannot connect to the server";
        return 1;
    }

/*    if((n=write(sockfd, &cliFrame, sizeof(cliFrame)))!=sizeof(cliFrame));

    if(n<0)
    {
        std::cerr<<"Error: cannot write to given socket"<<std::endl;
        return 0;   
    }

    recv(sockfd, &cliFrame, sizeof(cliFrame), MSG_WAITALL);*/

    return sockfd;
}

SFrame CConnectionHandler::frameCreator(int dataType, std::string messageData, std::string login, int destPort)
{
    SFrame frame;
    memset(&frame, 0, sizeof(frame));
    strcpy(frame.m_CID, login.c_str());
    frame.m_dataType=dataType;
    strcpy(frame.m_messageData, messageData.c_str());
    frame.m_destenationPort=destPort;

    return frame;
}

void CConnectionHandler::listening(int socketfd)
{
    SFrame frame;
    memset(&frame, 0, sizeof(frame));
    int dt=0;

    while(1)
    {
        recv(socketfd, &frame, sizeof(frame), MSG_WAITALL);
        dt=static_cast<int>(frame.m_dataType);

        switch(dt)
        {
            case 1:
                std::cout<<frame.m_messageData<<std::endl;
                //TODO:answering to situation
                flag=1;
                flagChanged = true;
                
                break;

            case 2:
                std::cout<<frame.m_messageData<<std::endl;
                //TODO:: answer handler
                sleep(1);
                break;

            case 3:
                std::cout<<frame.m_messageData<<std::endl;
                break;

            case 4:
                break;

            case 5:
                 break;       

            case 7:
                std::cout<<frame.m_messageData<<std::endl;
                //TODO:answering to situation
                flag=1;
                flagChanged = true;

                break;

            default:
                 break;
        }

    }
}
