//Taronish Daruwalla
#include "udp.h"
#include<iostream>
#include<cstdlib>
#include<csignal>
#include<string>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<netinet/in.h>
#include<dirent.h>
#include<sys/poll.h>
#include<vector>
#include<sstream>
#include<fcntl.h>

using namespace std;

void UDPClient::broadcast(int serverport){
		
}

UDPClient::UDPClient(){
	ServerFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(ServerFileDescriptor < 0){
		cerr << "Unable to call socket()" << endl;
		exit(1);
	}//socket call failed
}

UDPClient::~UDPClient(){

}
