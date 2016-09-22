#include "CMessageHandler.h"
#include "SFrame.h"
#include <sstream>
#include <string.h>
using namespace std;

CMessageHandler::CMessageHandler()
{}

CMessageHandler::CMessageHandler(string host)
	:m_host(host),
{}
CMessageHandler::~CMessageHandler()
{}

void CMessageHandler::sendToAll(CThPool *tp, Frame frame)
{
	struct Frame o_allFrame;
	for(auto it=tp->broad.begin(); it!=tp->broad.end(); ++it)
	{
		if(it->first==frame.m_CID)
		o_allFrame.m_messageData=frame.m_messageData;

	}
}

void CMessageHandler::broadcast(CThPool *tp, int port)
{
	struct Frame o_frame;
	tp->broad[login]=port;
	int dt=-1;
	while(1)
	{
		read(port, &o_frame, sizeof(o_frame));
		dt=static_cast<int>(o_frame.m_dataType);
		if(dt==0)
		{
			sentToAll(tp, o_frame);
		}
		else if(dt==5)
		{
			break;
		}
		else
		{
			//TODO:error handling
		}
	
	}
	tp->broad.erase(login);	

}

bool CMessageHandler::createChatRoom(Frame cliFrame, int cliSock)
{
	m_chatUsers[cliFrame.m_CID]=cliSock;	
	istringstream ss;
	ss.str(cliFrame.m_messageData);
	string login=cliFrame.m_CID;

	while(login.length()>0)
	{
		//TODO: get client sockets
		m_chatUsers[login]=m_tp->online[login];
		ss >> login;
	
	}

	chatRoomHandler();
	


}

void CMessageHandler::chatRoomHandler()
{


}
