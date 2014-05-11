#ifndef TCP_H
	#define TCP_H

#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <stdint.h>

using namespace std;

class TCPServer{
	int serverFileDescriptor;
	struct sockaddr_in serverAddress;
	uint16_t port;

public:
	
	TCPServer(uint16_t tcpport);
	~TCPServer();
};






class TCPClient{
	friend class TCPServer;

public:
	TCPClient();
	~TCPClient();
};

#endif

