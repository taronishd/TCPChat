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
	int serverFileDescriptor;
	struct sockaddr_in serverAddress;
	int currentTO, maxTO;
	int port;

public:
	void broadcast();
	UDPClient(int udpport, int initialto, int maxto);
	~UDPClient();

};//class UDPClient

#endif
