/**********************************************
TCP client wrapper for connect with TCP server
compile:
g++ -std=c++17 clienttcp.h clienttcp.cpp 
**********************************************/

#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;

class clienttcp
{
		int sock;
		struct sockaddr_in addr;
	
	public:
        clienttcp(char* host, int port);
		~clienttcp();
        int sendMsg(const char* msg,const size_t sizepackage);	
};

#endif
