#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "SFrame.h"
#include <iostream>

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	struct Frame cliFrame;

	bzero((char*)&cliFrame, sizeof(cliFrame));

	sprintf(cliFrame.m_CID, "kami");
	cliFrame.m_dataType=7;
	sprintf(cliFrame.m_messageData, "kami 123456");

	char buffer[256];

	if(argc < 3)
	{
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd<0)
	{
		perror("ERROR opening socket");
		exit(1);
	}

	server = gethostbyname(argv[1]);

	if(server ==NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);


	if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
	{
		perror("ERROR connecting");
		exit(1);
	}



	if((n=write(sockfd, &cliFrame, sizeof(cliFrame)))!=sizeof(cliFrame));
	
	if(n<0)
	{
		perror("ERROR writing to socket");
		exit(1);
	}


	read(sockfd, &cliFrame, sizeof(cliFrame) );

	std::cout<<cliFrame.m_messageData<<std::endl;

	cliFrame.m_dataType=7;
	sprintf(cliFrame.m_messageData, "tamii 123456");

	if((n=write(sockfd, &cliFrame, sizeof(cliFrame)))!=sizeof(cliFrame));
	
	if(n<0)
	{
		perror("ERROR writing to socket");
		exit(1);
	}

	read(sockfd, &cliFrame, sizeof(cliFrame) );

	std::cout<<cliFrame.m_messageData<<std::endl;

	return 0;
}
