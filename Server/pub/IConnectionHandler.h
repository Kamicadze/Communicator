#pragma once

class IConnectionHandler
{
	public:
		virtual ~IConnectionHandler(){}
		virtual void listening()=0;
		virtual void sendData()=0;
		virtual void clientHandler()=0;

};
