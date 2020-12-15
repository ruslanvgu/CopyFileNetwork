#include "servertcp.h"

servertcp::servertcp(char* host, int port)
{
	cout << "server" << endl;
	
	//create socket 
	sock = socket(AF_INET, SOCK_STREAM, 0);  
	if(sock < 0)
	{
		perror("error create socket");
		exit(1);
	}
	
	//init stuct  address
	addr.sin_family = AF_INET;				
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host);
	
	//communicate socket and address
	if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) 
	{
		perror("error bind");
		exit(2);	
	}
	
	//create queue querys
	if(listen(sock, 5) < 0) 
	{
		perror("error listen");
		exit(3);
	}
	
	cout << "Listen interface " << host <<" on port "<< port << endl;

	while(1)
	{
		struct sockaddr_in addrNew;
		unsigned int lengthAddrNew = sizeof(addrNew);
		sockNew = accept(sock, (struct sockaddr*)&addrNew, &lengthAddrNew);
		if(sockNew >= 0)
		{
			ClientData client(inet_ntoa(addrNew.sin_addr), addrNew.sin_port, sockNew);
			client.printData();
			arrayClient.push_back(client);

			std::thread th(&servertcp::newSession, this, sockNew, 2);//заменить auto myFuture = std::async(std::launch::async, myFunction)
			th.detach();
		} 
		else 
		{
			perror("error accept");
		}
	}
}

servertcp::~servertcp()
{

}

void servertcp::newSession(const int sock, const size_t sizepackage)
{
    //geting data
    cout << "thread №" <<  std::this_thread::get_id() << endl;
    int bytesRead = 0;
    char buf[sizepackage] = {0};
    std::string data;
    while((bytesRead = recv(sock, buf, sizepackage, 0)) > 0)
    {
    	std::string package(buf, sizepackage); 
        data +=  package;
    }

    //processing
    printToConsole(data);

    //close socket
    close(sock);
}

void servertcp::removeClient(const ClientData &client)
{
	int x = 0;
	for(const auto& i : arrayClient)
		if(i.sock == client.sock)
            arrayClient.erase(arrayClient.begin() + x++);
}

void servertcp::printToConsole(const std::string  &text)
{
    std::lock_guard<std::mutex> guard(mutex);
    cout << text << endl;
}
