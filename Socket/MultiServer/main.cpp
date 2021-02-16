#include "TCPMultiServer.h"

class CMyMultiServer : public CTCPMultiServer<int>
{
public:

    virtual void Connect(void) override
    {
        printf("connect\n");
    }

    virtual void DisConnect(void) override
    {
        printf("disconnect\n");
    }

    virtual void Recieve(const int& data, int size) override
    {
        printf("Recv  : %d\n", data);
        //printf("Size  : %d\n", size);
        //printf("error : %d\n", m_Error);
    }

};

signed main(void)
{
    CMyMultiServer intserver;
    intserver.Start();

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

	return 0;
}