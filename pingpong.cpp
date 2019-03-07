#include "pingpong.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <unistd.h>
#include <netdb.h>  
#include <cstdlib>
#include <iostream>


using namespace std;


int run_server(int port){

	int sockfd, newsockfd;
	unsigned int clilen;	
	
	struct sockaddr_in server_addr, cli_addr;
	
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		cout << "Socket error." << endl;
		exit(1);
	}
	
	memset((void *) &server_addr, 0, sizeof(server_addr));
	

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
	{
		cout << "Bind error." << endl;
		close(sockfd);
		return 1;
	}
	listen(sockfd,10);
	
	while(1) 
	{
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0) 
		{
			cout << "Accept error." << endl;
			exit(1);
		}
		handleConnection(newsockfd);
	}
	
	
	return sockfd;
}

int connect_to_server(char* wo, int port, ConnectionInfo* con){

	int sockfd;
	struct sockaddr_in server_addr;
	int msgSize;
	struct hostent* hent;
	

	if((hent=gethostbyname(wo)) == NULL) 
	{
		cout << "Invalid host name." << endl;
		exit(1);
	}
	

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		cout << "Socket error." << endl;
		exit(1);
	}
	
	memset((void *) &server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr = *((struct in_addr *)hent->h_addr);
	server_addr.sin_port = htons(port);

	if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
	{
		cout << "Connect error." << endl;
		return 1;
	}

	cout << "Ready to send messages" << endl;

	con->sockid=sockfd;

	return 0;

}

int sendMessage(ConnectionInfo* con, char* message){
	int msgSize = 0;
	
	if((msgSize = send(con->sockid, message, strlen(message), 0)) < 0) 
	{
		cout << "Send error." << endl;
		return 1;
	}

	return 0;

}

char* recieveMessage(ConnectionInfo* con){
	
	char response[4096];
	memset(response, '\0', 4096);

	int msgSize = 0;
	
	if((msgSize = recv(con->sockid, response, 4095, 0)) < 0) 
	{
		cout << "Receive error" << endl;
	}

	char* retval = (char*)malloc(sizeof(response)); 
	sprintf(retval, "%s", response);				

	return retval;
}

void dealocate_message(char* mem){
	free(mem);
}

void handleConnection(int clisock) 
{
	
	int msgSize;
	char buffer[1016]; // 
	memset(buffer, '\0', 1016);
	
	if((msgSize = recv(clisock, buffer, 1015, 0)) < 0) 
	{
		cout << "Receive error." << endl;
	}
	if(buffer[0]=='P' && buffer[1]=='I'&& buffer[2]=='N' && buffer[3]=='G' && buffer[4] == '\0')
	{
		buffer[0] = 'P';
		buffer[1] = 'O';
		buffer[2] = 'N';
		buffer[3] = 'G';
		buffer[4] = '\0';
	}
	if(buffer[0]=='p' && buffer[1]=='i'&& buffer[2]=='n' && buffer[3]=='g' && buffer[4] == '\0')
	{
		buffer[0] = 'p';
		buffer[1] = 'o';
		buffer[2] = 'n';
		buffer[3] = 'g';
		buffer[4] = '\0';
	}
	cout << "Message received from client: " << buffer<<endl;
	char response[1024];
	sprintf(response, "Server: I received the following message:  %s", buffer);
	
	if((msgSize = send(clisock, response, strlen(response), 0)) < 0) 
	{
		cout << "Send error." << endl;
	} 
	
	close(clisock);
}