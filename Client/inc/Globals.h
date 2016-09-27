#pragma once
#include <cstdlib>
#include "CConnectionHandler.h"

extern bool flagChanged;
extern int flag;

static void *gListen(void *parm)
{
    CConnectionHandler *ch=new CConnectionHandler();
    int sfd=*(int *)(parm);
    
    
    ch->listening(sfd);

}
