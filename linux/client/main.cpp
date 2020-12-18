#include "clienttcp.h"

int main(int argc, char* argv[] )
{

	if(argc < 2)
	{
        perror("input host and port {127.0.0.1 9000}");
        return 1;
    }

	clienttcp app(argv[1], atoi(argv[2]));
	//int i =0;
	
//	app.sendMsg(argv[3],2);
	//	app.sendMsg("hello",2);
		//std::this_thread::sleep_for (std::chrono::seconds(1));
	app.sendFile("/home/sintez/tmp/1.txt");


	return 0;
}