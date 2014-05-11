//Taronish Daruwalla
#include "tcp.h"
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/poll.h>
#include <vector>
#include <sstream>
#include <fcntl.h>
#define BUFF_SIZE 512

using namespace std;

//********** TCP Server Implementations ******************

TCPServer::TCPServer(uint16_t tcpport){
	serverFileDescriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(serverFileDescriptor < 0){
		cerr << "Unable to call socket() for tcp." << endl;
		exit(1);
	}
	bzero((char *) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(tcpport);
	
	int goodBind = bind(serverFileDescriptor, 
		(struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if(goodBind < 0){
		cout << "Error binding TCP socket" << endl;
		close(serverFileDescriptor);
		exit(1);
	}
	port = tcpport;
}


TCPServer::~TCPServer(){

}


//********** TCP Client Implementations *********************


TCPClient::TCPClient(){

}


TCPClient::~TCPClient(){

}



