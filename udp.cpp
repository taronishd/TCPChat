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

void UDPClient::broadcast(const uint16_t &tcpport){
	int broadcastEnable = 1, actualSize, setOptions, sentBuffer;
	uint8_t *buffer = new uint8_t[512];
	bzero(buffer, 512);
	setOptions = setsockopt(serverFileDescriptor, SOL_SOCKET, SO_BROADCAST,
		&broadcastEnable, sizeof(broadcastEnable));
	if(setOptions < 0){
		cerr << "Unable to set socket options." << endl;
		close(serverFileDescriptor);
		exit(1);
	}
	actualSize = assembleBroadcast(buffer, tcpport);
	sentBuffer = sendto(serverFileDescriptor, buffer, actualSize, 0, 
		(struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if(sentBuffer < 0){
		cerr << "Unable to call sendto()" << endl;
		close(serverFileDescriptor);
		exit(1);
	}

}


int UDPClient::assembleBroadcast(uint8_t buffer[], const uint16_t &tcpport){
	char *signature = "P2PI";
	uint16_t type = 0x0001;
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


UDPClient::UDPClient(uint16_t udpport, int initialto, int maxto){
	serverFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(serverFileDescriptor < 0){
		cerr << "Unable to call socket()" << endl;
		exit(1);
	}//socket call failed
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	serverAddress.sin_port = htons(udpport);
	port = udpport;
	currentTO = initialto;
	maxTO = maxto;
}

UDPClient::~UDPClient(){

}
