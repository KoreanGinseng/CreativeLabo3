#include "TCPClient.h"

struct GameData  : public DataHeader
{
    int         data;
};
struct GameData2 : public DataHeader
{
    std::string data;
};
struct GameData3 : public DataHeader
{
    float       data;
};

class CMyClient : public CTCPClient
{
public:

    CMyClient(const char* s) : CTCPClient(s) {}

    void Recieve(const DataHeader& h, const void* p, int s) override
    {
        printf("type : %d\n", h.Type);
        switch (h.Type)
        {
        case 0:
            printf("case 0: %d\n", *(reinterpret_cast<const int*>(p)));
            break;
        case 1:
            printf("case 1: %s\n", (reinterpret_cast<const std::string*>(p))->c_str());
            break;
        case 2:
            printf("case 2: %f\n", *(reinterpret_cast<const float*>(p)));
            break;
        }
    }
};

unsigned main(void)
{
    CMySocket::StartUp();
    CMyClient client("127.0.0.1");

    client.Start();

	//ëóêM
	while (TRUE)
	{
		static int data = 10;
		data--;
        if (data % 3 == 0)
        {
            GameData gd;
            gd.Type = 0;
            gd.Size = sizeof(int);
            gd.data = 100 * data;
            client.Send(&gd, sizeof(gd));
        }
        if (data % 3 == 1)
        {
            GameData2 gd;
            gd.Type = 1;
            gd.Size = sizeof(std::string);
            gd.data = "aaaa" + std::to_string(data);
            client.Send(&gd, sizeof(gd));
        }
        if (data % 3 == 2)
        {
            GameData3 gd;
            gd.Type = 2;
            gd.Size = sizeof(float);
            gd.data = 1.234f + (data * 0.0001f);
            client.Send(&gd, sizeof(gd));
        }
        printf("data : %d\n", data);

		if (data == -1)
		{
			printf("-1 Send End\n");
			break;
		}
		Sleep(1000);
	}

	Sleep(1000);
    CMySocket::CleanUp();
	return 0;
}