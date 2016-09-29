#include "CMenu.h"
#include "CConnectionHandler.h"
#include <iostream>
#include "Globals.h"
#include <pthread.h>

int main(int argc, char *argv[])
{
    
    if(argc < 3)
    {
        fprintf(stderr, "usage %s hostname port \n", argv[0]);
        return 1;
    }

  //  flagChanged=false;
  //  flag=0;

    int portno, socketfd;
    portno = atoi(argv[2]);
    CConnectionHandler ch;
    CMenu cm;
    socketfd=ch.handshake(portno, argv[1]);
    if(1==socketfd)
    {
        return 1;
    }
    cm.setSock(socketfd);

    pthread_t *thread=new pthread_t;
    pthread_create(thread, 0, gListen, &socketfd);

    cm.startingMenu();
    


   // pthread_join(thread);
    delete thread;
    return 0;

}
