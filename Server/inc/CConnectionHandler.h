#pragma once
#include "IConnectionHandler.h"
#include "ITask.h"
#include "CThPool.h"
#include "SFrame.h"
#include "CSystem.h"
#include "CDatabaseHandler.h"

class CConnectionHandler: public IConnectionHandler, public ITask
{
    private:
        int m_flag;
        int m_socketfd;
        int m_clisocket;
        IThPool *m_tp;
        ISystem *m_sys;
        SFrame m_clientFrame;
        IDatabaseHandler *m_dbh;
        FRIEND_TEST(Case1AuthenticationTrue, ClientHandling);
        
    public: 
        CConnectionHandler(int, IThPool*, int);
        CConnectionHandler(int, IThPool*, ISystem* );
        CConnectionHandler(int, int, SFrame, IDatabaseHandler*);
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
enum ERunFlag
{
    SHAKING=2,
    CLIENTHANDLING=3
};

enum ESwitchDataTypes
{
    LOGGING=1,
    DELETING=2,
    BROADCAST=3,
    CHATROOM=4,
    EXIT=5,
    JOININGCHAT=6,
    CREATINGUSER=7,
    PASSONLINE=8,
    SHUTDOWN=66
};

enum EErrorHandlers
{
    SUCCES=0,
    UNSUCCESFUL=1
};
