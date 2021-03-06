//Taronish Daruwalla, 997447158

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <csignal>
#include <stdlib.h>
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
#include <arpa/inet.h>
#include "udp.h"
#include "tcp.h"
#define BUFF_SIZE  514

using namespace std;

void parseParams(string &username, string &hostname, uint16_t &udpport,
	char *av[], uint16_t &tcpport, int &initialto, int &maxto, 
	uint16_t &hostport, int ac);
int pollAll(struct pollfd pollfds[], int &nfds, int &acceptedfd,
	const int &currentto);
void sigIntHandler(int param);
struct sockaddr_in getHostName(const char *hostname);

typedef struct{
	struct sockaddr_in address;
	char *userName;
}tcpClient;

bool timeToClose = false;

int main(int argc, char *argv[]){
	string userName, hostName;
	uint16_t udpPort = 50550, tcpPort = 50551, hostPort = -1;
	int initialTO = 5, maxTO = 60, nFDs = 2, pollResult;
	int acceptedFD, clientIndex = 0, type;
	tcpClient clients[64];
	bool gotBro = false;

	parseParams(userName, hostName, udpPort, argv, tcpPort, initialTO,
		maxTO, hostPort, argc);
	int currentTO = initialTO;

	signal(SIGINT, sigIntHandler);

	UDPClient udpClient(udpPort);
	if(hostPort != -1){
		new (&udpClient) UDPClient(hostPort);
	}
	TCPServer tcpServer(tcpPort);

	struct pollfd pollFDs[64];
	bzero(pollFDs, sizeof(pollFDs));
	pollFDs[0].fd = udpClient.getFD();
	pollFDs[0].events = POLLIN;
	pollFDs[1].fd = tcpServer.getFD();
	pollFDs[1].events = POLLIN;
	listen(tcpServer.getFD(), 16);

	while(1){
		if(!gotBro){
			//To send unicast discovery b/c of -pp, or broadcast
			//cout << "host port : " << hostPort << endl;
			if(hostPort == 65535){
				udpClient.sendDatagram(tcpPort, 0x0001, 
					udpClient.getServerAddress(), true);
			}
			else{
				udpClient.sendDatagram(tcpPort, 0x0001, 
					getHostName(hostName.c_str()), false);
			}
		}
		if(timeToClose){
			cout << "Closing..." << endl;
			udpClient.sendDatagram(tcpPort, 0x0003, udpClient.getServerAddress(),
				false);
			close(udpClient.getFD());
			close(tcpServer.getFD());
			exit(0);
		}
		pollResult = pollAll(pollFDs, nFDs, acceptedFD, currentTO);
		if(pollResult == -1)
		{
			close(udpClient.getFD());
			exit(1);
		}
		if(pollResult == 0)
		{
			if(currentTO*2 > maxTO)
				currentTO = maxTO;
			else
				currentTO *= 2;
			//close(udpClient.getFD());
			//close(tcpServer.getFD());
			//exit(0);
		}
		//got a unicast UDP datagram discovery or reply
		if(pollResult == 1){
			type = udpClient.parseMessage(clients[clientIndex].address, tcpPort);
			//if(type == 2){
				gotBro = true;
			//}
			//tcpServer.establishConnection(clients[clientIndex].address);
			clientIndex++;
		}
		//received a TCP segment
		if(pollResult == 2){

		}
	}//polling loop

	return 0;
}//main


void parseParams(string &username, string &hostname, uint16_t &udpport,
		char *av[], uint16_t &tcpport, int &initialto, int &maxto, 
		uint16_t &hostport, int ac){
	if(ac % 2 == 0){
		cerr << "Incorrect parameter format" << endl;
		exit(1);
	}

	for(int i = 1; i < ac; i+=2){
		if(strcmp(av[i], "-u") == 0){
			username = av[i+1];
		}
		if(strcmp(av[i], "-up") == 0){
			udpport = atoi(av[i+1]);
			if(udpport < 0 || udpport > 65535){
				cerr << "Invalid UDP port." << endl;
				exit(1);
			}
		}
		if(strcmp(av[i], "-tp") == 0){
			tcpport = atoi(av[i+1]);
			if(tcpport < 0 || tcpport > 65535){
				cerr << "Invalid UDP port." << endl;
				exit(1);
			}
		}
		if(strcmp(av[i], "-dt") == 0){
			initialto = atoi(av[i+1]);
		}
		if(strcmp(av[i], "-dm") == 0){
			maxto = atoi(av[i+1]);
		}
		if(strcmp(av[i], "-pp") == 0){
			hostname = strtok(av[i+1], ":");
			hostport = atoi(strtok(NULL, ""));
		}

	}
}//parse parameters


int pollAll(struct pollfd pollfds[], int &nfds, int &acceptedfd,
		const int &currentto){
	cout << "Polling with timeout of " << currentto << " secs." << endl;
	if(poll(pollfds, nfds, currentto*1000) < 0){
		//cout << "poll() failed..." << endl;
		//return -1;
	}
	if(pollfds[0].revents && pollfds[1].revents){
		cout << "UDP datagram and TCP segment recieved." << endl;
		return 3;
	}
	if(pollfds[0].revents & POLLIN){
		//cout << "datagram received" << endl;
		acceptedfd = accept(pollfds[0].fd, NULL, NULL);
		pollfds[nfds].fd = acceptedfd;
		pollfds[nfds].events = POLLIN;
		nfds++;
		return 1;
	}
	if(pollfds[1].revents & POLLIN){
		cout << "TCP segment recieved" << endl;
		return 2;
	}
	return 0;
}


struct sockaddr_in getHostName(const char *hostname){
	//struct in_addr addr;
	//inet_aton(hostname, &addr);
	struct sockaddr_in temp;
	//temp = (struct Sockaddr_in)addr;
	return temp;
}


void sigIntHandler(int param){
	cout << "in handler";
	if(param == SIGINT){
		timeToClose = true;		
	}
}
