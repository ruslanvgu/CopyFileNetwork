#include "servertcp.h"

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
        perror("input host and port {127.0.0.1 9000}");
        return 1;
    }

	servertcp app(argv[1], atoi(argv[2]));

	return 0;
}