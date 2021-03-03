#include "MultiServer.h"

struct GameData : public DataHeader
{
    int        data;
};
struct GameData2 : public DataHeader
{
    std::string data;
};
struct GameData3 : public DataHeader
{
    float       data;
};

class CMyMultiServer : public CMultiServer
{
public:
	explicit CMyMultiServer(Protocol prot, unsigned int multiCount = 10, int portNo = 18900, bool bStart = false)
		: CMultiServer(prot, multiCount, portNo, bStart)
	{
	}

    virtual void Connect(void) override
    {
        printf("connect\n");
    }

    virtual void DisConnect(void) override
    {
        printf("disconnect\n");
    }

    virtual void Recieve(const DataHeader& h, const void* p, int size) override
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
        case 3:
            Send(p, size);
            printf("case 3: BroadCast\n");
            break;
        }
    }

};

signed main(void)
{
    CMySocket::StartUp();
	CMyMultiServer server(Protocol::TCP);
    server.Start();

    printf("Error : %d\n", server.GetError());
    while (true)
    {
        SHORT escState = GetKeyState(VK_ESCAPE);
        if (escState & 0x8000)
        {
            break;
        }
    }

    printf("End\n");
    Sleep(1000);

    CMySocket::CleanUp();
	return 0;
}