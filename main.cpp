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
#include "udp.h"
#define BUFF_SIZE  514

using namespace std;

void parseParams(string &username, string &hostname, uint16_t &udpport,
	char *av[], uint16_t &tcpport, int &initialto, int &maxto, 
	uint16_t &hostport, int ac);

int pollAll(struct pollfd pollfds[], int &nfds, int &acceptedfd,
	const int &currentto);


int main(int argc, char *argv[]){
	string userName, hostName;
	uint16_t udpPort = 50550, tcpPort = 50551, hostPort;
	int initialTO = 5, maxTO = 60, nFDs = 1, pollResult;
	int acceptedFD, clientIndex = 0;
	struct sockaddr_in clientAddrs[64];

	parseParams(userName, hostName, udpPort, argv, tcpPort, initialTO,
		maxTO, hostPort, argc);
	int currentTO = initialTO;

	UDPClient udpClient(udpPort, initialTO, maxTO);

	struct pollfd pollFDs[64];
	bzero(pollFDs, sizeof(pollFDs));
	pollFDs[0].fd = udpClient.getFD();
	pollFDs[0].events = POLLIN;

	while(1){
		udpClient.broadcast(tcpPort);
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
		}
		if(pollResult == 1){
			udpClient.parseMessage(clientAddrs[clientIndex]);
			clientIndex++;
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
		cout << "poll() failed..." << endl;
		return -1;
	}
	if(pollfds[0].revents){
		cout << "datagram received" << endl;
		acceptedfd = accept(pollfds[0].fd, NULL, NULL);
		pollfds[nfds].fd = acceptedfd;
		pollfds[nfds].events = POLLIN;
		nfds++;
		return 1;
	}
	return 0;
}



