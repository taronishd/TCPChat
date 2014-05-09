#ifndef UDP_H
	#define UDP_H

#include <csignal>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <stdint.h>

using namespace std;

class UDPClient{
	int serverFileDescriptor;
	struct sockaddr_in serverAddress;
	int currentTO, maxTO;
	uint16_t port;

public:
	void broadcast(const uint16_t &tcpport);
	void assembleBroadcast(uint8_t buffer[], const uint16_t &tcpport);
	char* getUsername(int &size);
	char* getHostname(int &size);
	UDPClient(uint16_t udpport, int initialto, int maxto);
	~UDPClient();

};//class UDPClient

#endif
