#include "CMessageHandler.h"
#include "SFrame.h"
#include <sstream>
#include <string.h>
using namespace std;

CMessageHandler::CMessageHandler()
{}

CMessageHandler::CMessageHandler(string host)
	:m_host(host)
{}
CMessageHandler::~CMessageHandler()
{}

bool CMessageHandler::createChatRoom(Frame cliFrame, int cliSock)
{
	m_chatUsers[cliFrame.m_CID]=cliSock;	
	istringstream ss;
	ss.str(cliFrame.m_messageData);
	string login=cliFrame.m_CID;
	int tmp=123;

	while(login.length()>0)
	{
		//TODO: get client sockets
		m_chatUsers[login]=tmp;
		ss >> login;
	
	}

	chatRoomHandler();
	


}

void CMessageHandler::chatRoomHandler()
{


}
