#ifndef UDP_H
	#define UDP_H

#include<csignal>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/poll.h>
#include<netinet/in.h>

using namespace std;

class UDPClient{
	int ServerFileDescriptor;
	struct sockaddr_in ServerAddress;

public:
	void broadcast(int serverport);
	UDPClient();
	~UDPClient();

};//class UDPClient

#endif
