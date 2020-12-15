#include "clienttcp.h"

clienttcp::clienttcp(char* host, int port)
{
	cout << "client" << endl;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("error create socket");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(host);
    //addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("error connect");
		exit(2);
	}
	else
	{
		cout << "connecting to server "<< host << endl;
	}
}

clienttcp::~clienttcp()
{
 	close(sock);
	cout << "connecting close" << endl;
}

int clienttcp::sendMsg(const char* oldmsg, const size_t sizepackage)
{
	//const char *message = msg.c_str();
	size_t lenoldmsg = strlen(oldmsg);
	if(lenoldmsg == 0)
	{
		return -1;
	}
	
	size_t remains = (lenoldmsg > sizepackage) ? sizepackage - (lenoldmsg % sizepackage) : sizepackage - lenoldmsg;
	char message[lenoldmsg + remains] = {0};
	memcpy(message, oldmsg, lenoldmsg);
	size_t countsend = sizeof(message) / sizepackage;

	int i = 0;
	while(i < countsend)
	{
    	int resultSend = send(sock, message + (i * sizepackage), sizepackage, 0);
		if (resultSend < 0)
			perror("error send message");
		i++;
	}
	return 0;
}

