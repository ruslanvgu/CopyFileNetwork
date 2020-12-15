/**********************************************
TCP server wrapper for connect with TCP client

using:
c++17 (-std=c++17)
thread (-lpthread)

compile:
g++ -std=c++17 -lpthread servertcp.h servertcp.cpp 

**********************************************/
#ifndef SERVERTCP_H
#define SERVERTCP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <ctime>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

using std::cout;
using std::endl;

class ClientData
{
	public:
		inline static int number = 0;
		char* dateConnect;
		char* host;
		int port;
		int sock;
	
		ClientData(char* host, int port, int sock)
		{
			time_t now = time(0);
			dateConnect = ctime(&now);
			this->host = host;
			this->port = port;
			this->sock = sock;
			number++;
		}

		void printData()
		{
            cout << "Client "  <<  "№ " <<  number  <<  " Socket " <<  sock  <<  endl;
			cout << dateConnect << "host " << host << " port " << port << endl;
		}
};

class servertcp
{
        std::mutex mutex;
		int sock, sockNew;
		struct sockaddr_in addr;
		std::vector<ClientData> arrayClient;
        void newSession(const int sock, const size_t sizepackage);
		void removeClient(const ClientData &client); 
        void printToConsole(const std::string  &text);
		
	public:
		servertcp(char* host, int port);					
		~servertcp();
};

#endif
