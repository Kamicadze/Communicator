#include <map>
#include <iostream>
#include <string.h>
#include "SFrame.h"


class CMessageHandler
{
	private:
	map<std::string, int> m_chatUsers;
	std::string m_host;

	public:
	CMessageHandler();
	CMessageHandler(std::string);
	~CMessageHandler();
	bool createChatRoom(Frame, int);
	void chatRoomHandler();


}
