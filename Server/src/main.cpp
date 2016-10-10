#include "CWorkQueue.h"
#include "CThPool.h"
#include "CDatabaseHandler.h"
#include <iostream>
#include <cstdio>
#include <cstdint>
#include "Globals.h"
#include "CConnectionHandler.h"
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "CSystem.h"

void breakit(int signum);
void helpListenToFinish();


int main()
{
    struct sigaction sa; ///struct in here is a must have, otherwise sigaction
    ///treated as a method
    sa.sa_handler=breakit;
    sigemptyset(&sa.sa_mask);
    static const int THNUMBER=10;
    CSystem *sys=new CSystem();
    CWorkQueue *wq=new CWorkQueue();
    CThPool *tp= new CThPool(THNUMBER, wq, sys);

    if(tp)
    {
    tp->init();


        if(sys)
        {

            CConnectionHandler *ch=new CConnectionHandler(2, tp, sys);

            if(ch)
            {

                tp->addTask(ch);

            }
            else
            {
                std::cerr<<"Error: Memory not allocated"<<std::endl;
            }
        }
    }
    else
    {
        std::cerr<<"Error: Memory not allocated"<<std::endl;
    }

    while(false==endOfServerFlag)
    {
        //TODO: infinite loop which will break after getting kill sig	
        sigaction(SIGINT, &sa, NULL);        


    }
    tp->finish();
    helpListenToFinish();
    tp->clear();
    delete tp;
    return 0;
}
void breakit(int signum)
{
    std::cout<<"Server stoping"<<std::endl;
    endOfServerFlag=true;;

}
void helpListenToFinish()
{
    int sockfd=socket(AF_INET, SOCK_STREAM, 0);
    hostent *server=gethostbyname("localhost");
    sockaddr_in servAddr;

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family=AF_INET;

    memcpy(server->h_addr, &servAddr.sin_addr.s_addr, server->h_length);
    servAddr.sin_port=htons(5001);
    if(connect(sockfd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr))<0)
    {
        std::cerr<<"Emergancy shutdown failed. RUN AWAY!!!"<<std::endl;
    }
    std::cout<<"SUCCES!!"<<std::endl;
    close(sockfd);
}

