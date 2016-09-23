#include "CConnectionHandler.h"
#include <unistd.h>
//#include <sys/socked.h>
#include <netdb.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Globals.h"
#include "SFrame.h"
#include <sstream>
#include "CMessageHandler.h"
#include "CDatabaseHandler.h"
using namespace std;


CConnectionHandler::CConnectionHandler(int flag, CThPool *tp, int clisocket)
	:m_flag(flag),
	m_clisocket(clisocket),
	m_tp(tp)
{

}

CConnectionHandler::CConnectionHandler(int flag, CThPool *tp)
	:m_flag(flag),
	m_tp(tp)
{

}

CConnectionHandler::CConnectionHandler(int flag, CThPool *tp, int clisock, Frame cliFrame)
	:m_flag(flag),
	m_clisocket(clisock),
	m_tp(tp),
	m_clientFrame(cliFrame)
{}

CConnectionHandler::~CConnectionHandler()
{}

void CConnectionHandler::run()
{
	switch(m_flag)
	{

		case 2:
			listening();
			break;

		case 3:
			clientHandler();
			break;
	}

}

void CConnectionHandler::listening()
{
	int port, clilen, newsockfd, n;
	m_socketfd=socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serv_addr, cli_addr;
	Frame cliFrame;
	
	

	if(m_socketfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}

	bzero(reinterpret_cast<char*>(&serv_addr), sizeof(serv_addr));
	port=5001;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port= htons(port);

	if(bind(m_socketfd, (sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		perror("ERROR on binding");
		exit(1);
	}



	while(1)
	{

		listen(m_socketfd, 5);
		clilen = sizeof(cli_addr);

		newsockfd=accept(m_socketfd, (sockaddr *)&cli_addr, (socklen_t*)&clilen);
		if(newsockfd < 0)
		{
			perror("ERROR on accept");
			exit(1);
		}
	
		n=read(newsockfd, &cliFrame, sizeof(cliFrame));
		if(n<0)
		{
			//TODO: error handling
		}


		m_tp->addTask(new CConnectionHandler(3, m_tp, newsockfd, cliFrame));

		newsockfd=0;

		bzero(reinterpret_cast<char*>(&cliFrame), sizeof(cliFrame));


	}
	close(newsockfd);

		
}
void CConnectionHandler::clientHandler()
{
	string buff;
	istringstream ss;
	int dt=0;
	string login, password;
	CDatabaseHandler o_dbh;
	CMessageHandler o_mh;
	uint32_t tmp32;
	uint8_t tmp8;
	while(1)
	{
		buff=reinterpret_cast<char*>(m_clientFrame.m_messageData);
		ss.str(buff);	
		dt=static_cast<int>(m_clientFrame.m_dataType);



		switch(dt)
		{
			case 1:		///handshake case
				ss >> login >> password;
	
	
				if(true==o_dbh.authenticate(login, password))
				{
					m_tp->online[login]=m_clisocket;
				
					ss.str(string());	
					ss.str(m_clientFrame.m_CID);	
					ss >> m_clientFrame.m_DCID;
					sprintf(m_clientFrame.m_CID, "Server");
					tmp32=m_clientFrame.m_sourceAddress;
					m_clientFrame.m_sourceAddress=m_clientFrame.m_destenationAddress;
					m_clientFrame.m_destenationAddress=tmp32;

					tmp8=m_clientFrame.m_sourcePort;
					m_clientFrame.m_sourcePort=m_clientFrame.m_destenationPort;
					m_clientFrame.m_destenationPort=tmp8;

					sprintf(m_clientFrame.m_messageData,"Succes");

					write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));

				}
				else
				{
					ss.str(string());	
					ss.str(m_clientFrame.m_CID);	
					ss >> m_clientFrame.m_DCID;
					sprintf(m_clientFrame.m_CID, "Server");
					tmp32=m_clientFrame.m_sourceAddress;
					m_clientFrame.m_sourceAddress=m_clientFrame.m_destenationAddress;
					m_clientFrame.m_destenationAddress=tmp32;

					tmp8=m_clientFrame.m_sourcePort;
					m_clientFrame.m_sourcePort=m_clientFrame.m_destenationPort;
					m_clientFrame.m_destenationPort=tmp8;

					sprintf(m_clientFrame.m_messageData, "Wrong Login and/or password");

					write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));

				}

				break;

			case 2:

				ss >> login >> password;

	
				if(true==o_dbh.authenticate(login, password))
				{
					m_tp->online.erase(login);
					o_dbh.deleteUser(login, password);
					
					ss.str(string());	
					ss.str(m_clientFrame.m_CID);	
					ss >> m_clientFrame.m_DCID;
					sprintf(m_clientFrame.m_CID, "Server");
					tmp32=m_clientFrame.m_sourceAddress;
					m_clientFrame.m_sourceAddress=m_clientFrame.m_destenationAddress;
					m_clientFrame.m_destenationAddress=tmp32;
	
					tmp8=m_clientFrame.m_sourcePort;
					m_clientFrame.m_sourcePort=m_clientFrame.m_destenationPort;
					m_clientFrame.m_destenationPort=tmp8;
	
					sprintf(m_clientFrame.m_messageData,"Succes");
	
					write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
	
				}
				else
				{
					ss.str(string());	
					ss.str(m_clientFrame.m_CID);	
					ss >> m_clientFrame.m_DCID;
					sprintf(m_clientFrame.m_CID, "Server");
					tmp32=m_clientFrame.m_sourceAddress;
					m_clientFrame.m_sourceAddress=m_clientFrame.m_destenationAddress;
					m_clientFrame.m_destenationAddress=tmp32;
	
					tmp8=m_clientFrame.m_sourcePort;
					m_clientFrame.m_sourcePort=m_clientFrame.m_destenationPort;
					m_clientFrame.m_destenationPort=tmp8;
	
					sprintf(m_clientFrame.m_messageData, "Wrong Login and/or password");
	
					write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
	
				}
	
				break;
	
			case 3:
	
				o_mh.broadcast(m_tp, m_clisocket, m_clientFrame.m_CID);
	
				break;
	
			case 4:
				o_mh.createChatRoom(m_clientFrame, m_clisocket, m_tp);
				break;
	
			case 5:		///goodbye case
				m_tp->online.erase(login);
				close(m_clisocket);
				break;
	
			case 6:
				//TODO: joining chat room
				break;
	
			default:
					
				ss.str(string());	
				ss.str(m_clientFrame.m_CID);	
				ss >> m_clientFrame.m_DCID;
				sprintf(m_clientFrame.m_CID, "Server");
				tmp32=m_clientFrame.m_sourceAddress;
				m_clientFrame.m_sourceAddress=m_clientFrame.m_destenationAddress;
				m_clientFrame.m_destenationAddress=tmp32;
	
				tmp8=m_clientFrame.m_sourcePort;
				m_clientFrame.m_sourcePort=m_clientFrame.m_destenationPort;
				m_clientFrame.m_destenationPort=tmp8;
	
				sprintf(m_clientFrame.m_messageData, "ERROR");
	
				write(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
				break;

			}
		
		bzero(reinterpret_cast<char*>(&m_clientFrame), sizeof(m_clientFrame) );
		read(m_clisocket, &m_clientFrame, sizeof(m_clientFrame));
	}
}

