#include "TCPClient.h"

unsigned main(void)
{
	CTCPClient<int> client("127.0.0.1");

    client.Start();

	//‘—M
	while (TRUE)
	{
		static int data = 10;
		data--;
		int s = client.Send(data);
		printf("%d Bytes Send\n", s);
		printf("%d Send\n", data);
		if (data == -1)
		{
			printf("-1 Send End\n");
			break;
		}
		Sleep(1000);
	}

	Sleep(1000);
	return 0;
}