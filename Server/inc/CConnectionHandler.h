#pragma once
#include "IConnectionHandler.h"
#include "ITask.h"
#include "CThPool.h"
#include "SFrame.h"
#include "CSystem.h"

class CConnectionHandler: public IConnectionHandler, public ITask
{
    private:
        int m_flag;
        int m_socketfd;
        int m_clisocket;
        IThPool *m_tp;
        ISystem *m_sys;
        SFrame m_clientFrame;
        
    public: 
        CConnectionHandler(int, IThPool*, int);
        CConnectionHandler(int, IThPool*, ISystem* );
        CConnectionHandler(int, int, SFrame);
        CConnectionHandler(){};
        virtual ~CConnectionHandler();
        int binding(sockaddr_in&);
        int listening();
        void run();
        int clientHandler();
        int socketCreator();
        int writeAnswer(std::string, int);
        int handshake();
        int accepting(int&, sockaddr_in&, int&);
        int recving(SFrame&, int&);

};
