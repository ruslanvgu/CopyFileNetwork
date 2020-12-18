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
	if(lenoldmsg == 0 || sizepackage < 0)
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

int clienttcp::sendFile(const char* pathFile)
{
	if(strlen(pathFile) == 0)
		return -1;
	
	std::ifstream file(pathFile);
	if(!file.is_open())
	{
		cout << "File " << pathFile << " not open\n" << endl;
		return -1;
	}

	file.seekg(0, std::ios::end);
	int sizeFile = file.tellg();
	file.seekg(0, std::ios::beg);

	char buff[sizeFile] = {0};	
	file.read(buff,sizeFile);
	file.close();
	
	if(sizeFile == 0)
	{
		cout << "File is empty" << endl;
		return -1;
	}
	
	int resultSend;
	int maxSize = 255;
	if(sizeFile <= maxSize)  //unsigned char
	{  
		uint8_t fieldSize = sizeFile;   	// size data in package <= 255
		uint8_t data[sizeFile + 1]; 		// [0 byte - sizeFile] [1-256 byte - dataFile]

		data[0] = fieldSize;				//init field size file
		for( int i = 0; i <= sizeFile; ++i ) //init field data
		{
			data[i + 1] = buff[i];	
		}

		resultSend = write(sock, data, sizeof(data));
		if (resultSend < 0)
		 	perror("error send package");
	} else {
		int countSend = sizeFile / maxSize; 
		int remains = sizeFile % maxSize;
		int i = 0;
		
		while(i < countSend)
		{
			uint8_t data[maxSize + 1]; 		// [0 byte - sizeFile] [1-256 byte - dataFile]
			data[0] = maxSize;
			
			int shift = maxSize * i;
			for( int j = 0; j < maxSize; ++j ) //init field data
			{
				data[j + 1] = buff[j + shift];	
			}

			resultSend = write(sock, data, maxSize);
			cout << resultSend << data << endl;
			if (resultSend < 0)
		 		perror("error send package " + i);
		 	i++;
		}
		if(remains > 0)  //last data
		{	
			uint8_t data[remains + 1];
			data[0] = remains;
			for( int j = 0; j < remains; ++j ) //init field data
			{
				data[j + 1] = buff[j + (sizeof(buff) - remains)];	
			}
			
			resultSend = write(sock, data, sizeof(data));
			cout << resultSend << data << endl;
			if (resultSend < 0)
		 		perror("error send package ");
		}
	}
	// size_t remains = (sizeFile > sizepackage) ? sizepackage - (sizeFile % sizepackage) : sizepackage - sizeFile;
	// char data[sizeFile + remains] = {0};
	// memcpy(data, buff, sizeFile);
	// size_t countsend = sizeof(data) / sizepackage;

	// int i = 0;
	// while(i < countsend)
	// {
 //    	int resultSend = send(sock, data + (i * sizepackage), sizepackage, 0);
	// 	if (resultSend < 0)
	// 		perror("error send message");
	// 	i++;
	// }
	
	return 0;
}