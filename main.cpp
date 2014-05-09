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

void parseParams(string &username,string &hostname,int &udpport,char *av[],
	int &tcpport, int &initialto, int &maxto, int &hostport, int ac);



int main(int argc, char *argv[]){
	string userName, hostName;
	int udpPort = 50550, tcpPort = 50551, initialTO = 5, maxTO = 60, hostPort;

	parseParams(userName, hostName, udpPort, argv, tcpPort, initialTO,
		maxTO, hostPort, argc);

	UDPClient udpClient(udpPort, initialTO, maxTO);
	udpClient.broadcast();

	return 0;
}//main


void parseParams(string &username,string &hostname,int &udpport,char *av[],
		int &tcpport, int &initialto, int &maxto, int &hostport, int ac){
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
