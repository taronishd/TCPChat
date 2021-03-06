//Taronish Daruwalla
#include "udp.h"
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

void UDPClient::sendDatagram(const uint16_t &tcpport, uint16_t type,
		struct sockaddr_in clientaddr, bool isBroadcast){
	int broadcastEnable = 1, actualSize, setOptions, sentBuffer;
	uint8_t *buffer = new uint8_t[512];
	bzero(buffer, 512);
	if(type == 0x0001){
		setOptions = setsockopt(serverFileDescriptor, SOL_SOCKET, SO_BROADCAST,
			&broadcastEnable, sizeof(broadcastEnable));
		if(setOptions < 0){
			cerr << "Unable to set socket options." << endl;
			close(serverFileDescriptor);
			exit(1);
		}
	}//if broadcast, set options
	actualSize = assembleDatagram(buffer, tcpport, type, clientaddr);
	if(type == 0x0001 && isBroadcast){
		sentBuffer = sendto(serverFileDescriptor, buffer, actualSize, 0, 
			(struct sockaddr *)&serverAddress, sizeof(serverAddress));
	}
	else{
		sentBuffer = sendto(serverFileDescriptor, buffer, actualSize, 0, 
			(struct sockaddr *)&clientaddr, sizeof(clientaddr));
	}
	if(sentBuffer < 0){
		cerr << "Unable to call sendto()" << endl;
		close(serverFileDescriptor);
		exit(1);
	}

}


int UDPClient::assembleDatagram(uint8_t buffer[], const uint16_t &tcpport,
		uint16_t type, struct sockaddr_in clientaddr){
	const char *signature = "P2PI";
	//uint8_t buffer[BUFF_SIZE];
	int unameSize, hostSize;
	char *username = new char[64];
	char *hostname = new char[64];
	username = getUsername(unameSize);
	hostname = getHostname(hostSize);
	//cout << username << endl << unameSize << endl;
	//cout << hostname << endl << hostSize << endl;
	for(int i = 0; i < 4; i++){ 
		buffer[i] = signature[i]; 
	}
	buffer[5] = type;	
	buffer[4] = type >> 8;
	buffer[7] = port;
	buffer[6] = port >> 8;
	buffer[9] = tcpport;
	buffer[8] = tcpport >> 8;
	memcpy(&(buffer[10]), hostname, hostSize);
	buffer[10 + hostSize] = '\0';
	memcpy(&(buffer[11 + hostSize]), username, unameSize);
	buffer[11 + hostSize + unameSize] = '\0';
	return 12 + hostSize + unameSize;
}

char* UDPClient::getUsername(int &size){
	int j;
	char *uname = new char[64];
	for(int i = 0; i < 64; i++)
		uname[i] = ' ';
	getlogin_r(uname, 64);
	for(j = 0; j < 64 && uname[j] != ' '; j++){}
	size = j - 1;
	return uname;
}

char* UDPClient::getHostname(int &size){
	int j;
	char *hname = new char[64];
	for(int i = 0; i < 64; i++)
		hname[i] = ' ';
	gethostname(hname, 64);
	for(j = 0; j < 64 && hname[j] != ' '; j++){}
	size = j - 1;
	return hname;
}

int UDPClient::getFD(){
	return serverFileDescriptor;
}

struct sockaddr_in UDPClient::getServerAddress(){
	return serverAddress;
}

int UDPClient::parseMessage(struct sockaddr_in &clientaddr, 
		uint16_t tcpport){
	socklen_t clientLength = sizeof(clientaddr);
	uint8_t buffer[BUFF_SIZE];
	int filledBuffer = recvfrom(serverFileDescriptor, buffer, BUFF_SIZE, 0,
		(struct sockaddr *)&clientaddr, &clientLength);
	if(filledBuffer < 0){
		cerr << "Unable to call recvfrom()" << endl;
		close(serverFileDescriptor);
		exit(1);
	}//in case recvfrom fails
	uint8_t type = buffer[5];
	cout << "got a datagram:" << endl;
	if(type == 0x01){
		sendDatagram(tcpport, 0x0002, clientaddr, false);
		cout << "Got discovery" << endl;
		return 1;
	}
	if(type == 0x02){
		cout << "Got reply." << endl;
		return 2;
	}
	if(type == 0x03){
		cout << "Got closing msg." << endl;
		return 3;
	}
	return -1;
}



UDPClient::UDPClient(uint16_t udpport){
	serverFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(serverFileDescriptor < 0){
		cerr << "Unable to call socket()" << endl;
		exit(1);
	}//socket call failed
	bzero((char *) &serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	serverAddress.sin_port = htons(udpport);
	int goodBind = bind(serverFileDescriptor, 
		(struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if(goodBind < 0){
		cout << "Error binding UDP socket" << endl;
		close(serverFileDescriptor);
		exit(1);
	}
	port = udpport;
}

UDPClient::~UDPClient(){

}
