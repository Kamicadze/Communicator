#include "CWorkQueue.h"
#include "CThPool.h"
#include "CDatabaseHandler.h"
#include <iostream>
#include <cstdio>
#include <cstdint>
#include "Globals.h"
#include "CConnectionHandler.h"
#include <signal.h>

static uint16_t loop;

void breakit(int signum);


int main()
{
    struct sigaction sa; ///struct in here is a must have, otherwise sigaction
                         ///treated as a method
    sa.sa_handler=breakit;
    sigemptyset(&sa.sa_mask);
    loop =1;
    static const int thNumber=10;
    CThPool *tp= new CThPool(thNumber);
    if(tp)
    {
        CConnectionHandler *ch=new CConnectionHandler(2, tp);
        if(ch)
        {
            tp->addTask(ch);
        }
        else
        {
            std::cerr<<"Error: Memory not allocated"<<std::endl;
        }
    }
    else
    {
        std::cerr<<"Error: Memory not allocated"<<std::endl;
    }

    while(1)
    {
		//TODO: infinite loop which will break after getting kill sig	
     //   sigaction(SIGINT, &sa, NULL);        
           
        
    }
    tp->finish();
    delete tp;
    return 0;
}
void breakit(int signum)
{
   std::cout<<"Server stoping"<<std::endl;
    loop=0;

}

