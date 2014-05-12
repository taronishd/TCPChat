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

int TCPServer::getFD(){
	return serverFileDescriptor;
}

void TCPServer::establishConnection(struct sockaddr_in clientaddr){
	uint8_t buffer[BUFF_SIZE];
	int sentSuccessful, segmentSize;
	segmentSize = assembleSegment(buffer, 0x0004);

	sentSuccessful = sendto(serverFileDescriptor, buffer, segmentSize, 0,
		(struct sockaddr *)&clientaddr, sizeof(clientaddr));
	if(sentSuccessful < 0){
		cout << "Could not send establish communication message." << endl;
		close(serverFileDescriptor);
		exit(1);
	}
}

int TCPServer::assembleSegment(uint8_t buffer[], uint16_t type){
	int unameSize = 0;
	const char *signature = "P2PI";
	char *userName = new char[64];
	if(type == 0x0004){
		for(int i = 0; i < 4; i++){
			buffer[i] = signature[i];
		}		
	}
	buffer[5] = type;
	buffer[4] = type >> 8;

	if(type == 0x0004){
		userName = getUsername(unameSize); 
		memcpy(&(buffer[6]), userName, unameSize);
	}

	return 6 + unameSize;
}


char* TCPServer::getUsername(int &size){
	int j;
	char *uname = new char[64];
	for(int i = 0; i < 64; i++)
		uname[i] = ' ';
	getlogin_r(uname, 64);
	for(j = 0; j < 64 && uname[j] != ' '; j++){}
	size = j - 1;
	return uname;
}

char* TCPServer::getHostname(int &size){
	int j;
	char *hname = new char[64];
	for(int i = 0; i < 64; i++)
		hname[i] = ' ';
	gethostname(hname, 64);
	for(j = 0; j < 64 && hname[j] != ' '; j++){}
	size = j - 1;
	return hname;
}




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



