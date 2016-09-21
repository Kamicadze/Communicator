#pragma once
#include "IConnectionHandler.h"
#include "ITask.h"
#include "CThPool.h"
#include "SFrame.h"

class CConnectionHandler: public IConnectionHandler, public ITask
{
	private:
		int m_flag;
		int m_socketfd;
		int m_clisocket;
		CThPool *m_tp;
		struct Frame m_clientFrame;
	public:	
		CConnectionHandler(int, CThPool*, int);
		CConnectionHandler(int, CThPool*);
		CConnectionHandler(int, CThPool*, int, Frame);
		CConnectionHandler(){};
		~CConnectionHandler();
		void listening();
		void run();
		void sendData();
		void clientHandler();
};
