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

using namespace std;

void UDPClient::broadcast(){
	int broadcastEnable = 1, setOptions;
	setOptions = setsockopt(serverFileDescriptor, SOL_SOCKET, SO_BROADCAST,
		&broadcastEnable, sizeof(broadcastEnable));
	if(setOptions < 0){
		cerr << "Unable to set socket options." << endl;
		close(serverFileDescriptor);
		exit(1);
	}

}

UDPClient::UDPClient(int udpport, int initialto, int maxto){
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
