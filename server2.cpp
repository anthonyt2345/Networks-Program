//BY: ANTHONY TAYLOR
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

using namespace std;

void handleConnection(int clisock) 
{
	int go = 1;
	
	while(go == 1)
	{
	int msgSize;
	int numSpace;
	int numSpace2;
	int contLegt;
	string contInput2;
	string colon = ": ";
	string contInput;
	string proStrg;
	string proStrg2;
	string proMess;
	string coma;
	string coma2;
	string fileMess;
	string fileMess2;
	string errorMess;
	char proArry2[5000];
	memset(proArry2, '\0', 5000);
	char proArry[5000];
	memset(proArry, '\0', 5000);
	char mystring [5000];
	memset(mystring, '\0', 5000);
	if((msgSize = recv(clisock, proArry, 5000, 0)) < 0) 
	{
		cout << "Send error." << endl;
		return;
	}
	
	proStrg = proArry;
	//cout << proStrg << endl;
	numSpace = (proStrg.find(":"));
	coma = (proStrg.substr(0,numSpace));
	fileMess = (proStrg.substr(numSpace+1));
	
	//cout << fileMess << endl;
	
	
	if(proStrg.compare("quit") == 0 )
	{
		go = 0;
	}
	
	if(coma.compare("STOR") == 0 )
	{
		proMess = "CTS:"+fileMess;
		FILE * pFile;
		pFile = fopen(fileMess.c_str(),"r");
		if (pFile)
		{
			errorMess = "ERR:403 Error This File Already exist";
			if((msgSize = send(clisock, errorMess.c_str(), errorMess.length(), 0)) < 0) 
			{
			cout << "Send error." << endl;
			return;
			}
			
		}
		else if((msgSize = send(clisock, proMess.c_str(), proMess.length(), 0)) < 0) 
			{
			cout << "Send error." << endl;
			return;
			}
			
		if((msgSize = recv(clisock, proArry2, 5000, 0)) < 0) 
	{
		cout << "Send error." << endl;
		return;
	}
	proStrg2 = proArry2;
	numSpace2 = (proStrg2.find(" "));
	coma2 = (proStrg2.substr(0,numSpace2));
	fileMess2 = (proStrg2.substr(numSpace2+1));
	
	FILE * cFile;
	cFile = fopen (fileMess.c_str(),"w");
	if (cFile!=NULL)
	{
    fputs (fileMess2.c_str(),cFile);
    fclose (cFile);
	}
	
	}
	
	if(coma.compare("RTRV") == 0 )
	{
		
		FILE * dFile;
		dFile = fopen(fileMess.c_str(),"r");
		if (!dFile)
		{
			errorMess = "ERR:069 Error This File doesn't exist";
			if((msgSize = send(clisock, errorMess.c_str(), errorMess.length(), 0)) < 0) 
			{
			cout << "Send error." << endl;
			return;
			}
		
		}
		else
		{
			
			
			if ( fgets (mystring , 5000 , dFile) != NULL )
			fclose (dFile);
		
			contInput = mystring;
			contLegt = (contInput.length());
			string size = to_string(contLegt);
			contInput2 = ("CONT:" + size + colon + contInput );
			if((msgSize = send(clisock, contInput2.c_str(), contInput2.length(), 0)) < 0) 
			{
			cout << "Send error." << endl;
			return;
			}
			
		}
		
			
		
	}
	
	}
	
	close(clisock);
	
}


int main(int argc, char* argv[]) 
{
	
	
	int sockfd, newsockfd;
	unsigned int clilen;
	int port = atoi(argv[1]);	
	struct sockaddr_in server_addr, cli_addr;
	
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		cerr << "Socket error." << endl;
		exit(1);
	}
	
	memset((void *) &server_addr, 0, sizeof(server_addr)); 
	
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);
	
	
	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
	{
		cerr << "Bind error.";
		exit(1);
	}
	
	
	listen(sockfd, 10);
	
	
	while(1) 
	{
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd < 0) 
		{
			cerr << "Accept error." << endl;
			exit(1);
		}
		handleConnection(newsockfd);
	}
	
	close(sockfd);
}

