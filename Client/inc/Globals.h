#pragma once
#include <cstdlib>
#include "CConnectionHandler.h"
#include <list>
#include <map>

extern bool flagChanged;
extern int flag;
extern std::list<std::string> pendingInvites;
extern std::map<std::string, int> chatMap;

static void *gListen(void *parm)
{
    CConnectionHandler *ch=new CConnectionHandler();
    int sfd=*(static_cast<int*>(parm));
    
    
    ch->listening(sfd);

}
