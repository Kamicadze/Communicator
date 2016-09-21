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
#include "CDatabaseHandler.h"
using namespace std;


CConnectionHandler::CConnectionHandler(int flag, CThPool *tp, int clisocket)
	:m_tp(tp),
	m_flag(flag),
	m_clisocket(clisocket)
{

}

CConnectionHandler::CConnectionHandler(int flag, CThPool *tp)
	:m_tp(tp),
	m_flag(flag)
{

}

CConnectionHandler::CConnectionHandler(int flag, CThPool *tp, int clisock, Frame cliFrame)
	:m_flag(flag),
	m_tp(tp),
	m_clisocket(clisock),
	m_clientFrame(cliFrame)
{}

CConnectionHandler::~CConnectionHandler()
{}

void CConnectionHandler::run()
{
	switch(m_flag)
	{
		case 1:
			sendData();
			break;
		case 2:
			listening();
			break;

		case 3:
			clientHandler();
			break;
		
	
	}

}

void CConnectionHandler::sendData()
{
	char buffer[256];

	int i=0;
	while(1){
	write(m_clisocket, "got your data", 14);
	read(m_clisocket, buffer, 255);
	pthread_mutex_lock(&console_mutex);
	printf("data: %s %d \n", buffer, pthread_self());	
	bzero(buffer, 256);
	pthread_mutex_unlock(&console_mutex);
	}
	
}

void CConnectionHandler::listening()
{
	int port, clilen, newsockfd, n;
	m_socketfd=socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv_addr, cli_addr;
	struct Frame cliFrame;
	char buffer[256];
	
	

	if(m_socketfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}

	bzero((char*) &serv_addr, sizeof(serv_addr));
	port=5001;

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port= htons(port);

	if(bind(m_socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		perror("ERROR on binding");
		exit(1);
	}



	while(1)
	{

	listen(m_socketfd, 5);
	clilen = sizeof(cli_addr);

	newsockfd=accept(m_socketfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen);

	if(newsockfd < 0)
	{
		perror("ERROR on accept");
		exit(1);
	}

	n=read(newsockfd, &cliFrame, sizeof(cliFrame));


	m_tp->addTask(new CConnectionHandler(3, m_tp, newsockfd, cliFrame));

	newsockfd=NULL;

	bzero((char*) &cliFrame, sizeof(cliFrame));


	}
	close(newsockfd);

		
}
void CConnectionHandler::clientHandler()
{
	string buff(reinterpret_cast<char*>(m_clientFrame.m_messageData));
	istringstream ss;
	ss.str(buff);	
	int dt=static_cast<int>(m_clientFrame.m_dataType);
	string login, password;
	CDatabaseHandler o_dbh;
	uint32_t tmp32;
	uint8_t tmp8;



	switch(dt)
	{
		case 1:
			//TODO: authentication

			ss >> login >> password;


			if(true==o_dbh.authenticate(login, password))
			{
				//usersOnline.push_back(login);
				
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
			//TODO: server option
			cout<<"";
			break;
		
		case 3:
			//TODO: broadcast msg
			cout<<"";
			break;

		case 4:
			cout<<"";
			//TODO: chat room msg
			break;
	
	
	}
	
}

