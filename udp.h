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
	void sendDatagram(const uint16_t &tcpport, uint16_t type, 
		struct sockaddr_in clientaddr);
	int assembleDatagram(uint8_t buffer[], const uint16_t &tcpport,
		uint16_t type, struct sockaddr_in clientaddr);
	char* getUsername(int &size);
	char* getHostname(int &size);
	int getFD();
	struct sockaddr_in getServerAddress();
	void parseMessage(struct sockaddr_in &clientaddr, uint16_t tcpport);
	UDPClient(uint16_t udpport, int initialto, int maxto);
	~UDPClient();

};//class UDPClient

#endif
