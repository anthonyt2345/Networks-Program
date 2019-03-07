
//BY: ANTHONY TAYLOR
#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

int main(int argc, char* argv[]) 
{
	char proArry[5000];
	char proArry2[5000];
	int sockfd;
	struct sockaddr_in server_addr;
	int msgSize, port;
	int contLegt;
	string contInput;
	string contInput2;
	string comInput;
	string usrInput;
	string filename;
	string errorMess;
	string fileCont;
	string colon = ": ";
	char output[1024]; // Output message from server.
	struct hostent* hent;
	memset(output, '\0', 1024);
	int go = 1;
	int lov = 1;
	char buffer[30];
	
	
	

	if(argc < 3) 
	{
		cerr << "Usage: ./client [server name] [port]" << endl;
		exit(1);
	}
	
	port = atoi(argv[2]);
	

	if(port <= 10000) 
	{
		cerr << "Port > 10000 required." << endl;
		exit(1);
	}
	
	// Error check the server name.
	if((hent=gethostbyname(argv[1])) == NULL) 
	{
		cerr << "Invalid host name." << endl;
		exit(1);
	}
	
	// Create the client socket.
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		cerr << "Socket error." << endl;
		exit(1);
	}
	
	memset((void *) &server_addr, 0, sizeof(server_addr)); // Clear the server address structure.
	
	// Set up the server address structure.
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr = *((struct in_addr *)hent->h_addr);
	server_addr.sin_port = htons(port);

	if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
	{
		cerr << "Connect error." << endl;
		exit(1);
	}
	
	while(go == 1)
	{	
	cout << "\n";
	cout <<"Enter put <filename> to run a put command  : \n";
	cout <<"Enter get <filename> to run a get command  : \n";
	cout <<"Enter quit to run a quit command  : \n";
	cout <<"ENTER A COMMAND: \n";
	
	getline(cin,comInput);
	
	int numSpace = (comInput.find(" "));
	usrInput = (comInput.substr(0,numSpace));
	filename = (comInput.substr(numSpace+1));
	string proMess;
	
	if(usrInput.compare("put") == 0 || usrInput.compare("PUT") == 0)
	{
		proMess = ("STOR:" + filename);
		
		if((msgSize = send(sockfd, proMess.c_str(), proMess.length(), 0)) < 0) 
		{
		cout << "Send error."<< endl;
		return 1;
		}
		
		if((msgSize = recv(sockfd, proArry, 5000, 0)) < 0) 
		{
			
		cout << "Send error." << endl;
		return 1;
		
		}
		
		string messClear=proArry;
		int numSpace2 = (messClear.find(":"));
		string clearM = (messClear.substr(0,numSpace));
		//cout << clearM << endl;
		
		if(clearM.compare("ERR") == 0)
		{
			cout << messClear << endl;
		}
		
			FILE * pFile;
			char mystring [5000];

			pFile = fopen(filename.c_str(), "r");
			if (pFile == NULL) perror ("Error opening file");
			else 
			{
			if ( fgets (mystring , 5000 , pFile) != NULL )
			fclose (pFile);
			}
		
			contInput = mystring;
			contLegt = (contInput.length());
			string size = to_string(contLegt);
			contInput2 = ("CONT:" + size + colon + contInput );
			//cout << contInput2 << endl;
		
			if((msgSize = send(sockfd, contInput2.c_str(), contInput2.length(), 0)) < 0) 
			{
			cout << "Send error."<< endl;
			return 1;
			}
			
	}
	
	else if(usrInput.compare("get") == 0 || usrInput.compare("GET") == 0)
	{
		proMess = ("RTRV:" + filename);
		
		if((msgSize = send(sockfd, proMess.c_str(), proMess.length(), 0)) < 0) 
		{
		cout << "Send error."<< endl;
		return 1;
		}
		
		if((msgSize = recv(sockfd, proArry2, 5000, 0)) < 0) 
		{
			
		cout << "Send error." << endl;
		return 1;
		}
		 //cout << proArry2 << endl;
		
		string messClear2 = proArry2;
		int numSpace4 = (messClear2.find(":"));
		string clearM2 = (messClear2.substr(0,numSpace));
		
		if(clearM2.compare("ERR") == 0)
		{
			cout << messClear2 << endl;
		}
		else 
		{
		int numSpace3 = (messClear2.find(" "));
		fileCont = (messClear2.substr(numSpace3+1));
		
		FILE * aFile;
		aFile = fopen (filename.c_str(),"w");
		if (aFile!=NULL)
		{
		fputs (fileCont.c_str(),aFile);
		fclose (aFile);
		}
		}
		
	}
	
	else if(usrInput.compare("quit") == 0 || usrInput.compare("QUIT") == 0)
	{
		if((msgSize = send(sockfd, usrInput.c_str(), usrInput.length(), 0)) < 0) 
		{
		cout << "Send error."<< endl;
		return 1;
		}
		go = 0;
	}
	
	}
	
close(sockfd);
}

