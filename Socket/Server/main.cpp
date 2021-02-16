#include "TCPServer.h"

unsigned main(void)
{
	CTCPServer<int> tserver;

	//óM
	while (TRUE)
	{
		int Data = tserver.Recieve();
		printf("%d Receive\n", Data);
		if (Data == -1)
		{
			printf("-1 Receive End\n");
			break;
		}
	}

	return 0;
}
