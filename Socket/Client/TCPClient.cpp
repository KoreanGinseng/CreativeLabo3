#include "TCPClient.h"

CTCPClient::CTCPClient(const char * ip, int portNo)
{
    m_IP = ip;
    m_Socket.SetPortNo(portNo);
}

CTCPClient::~CTCPClient(void)
{
    m_Socket.CloseSocket();
    //�X���b�h�̒�~�ҋ@
    WaitForSingleObject(m_hRecieveThread, INFINITE);
    CloseHandle(m_hRecieveThread);
}

void CTCPClient::Start(const char * ip)
{
    m_IP = ip;
    Start();
}

void CTCPClient::Start(void)
{
    m_Socket.Start();
    //�\�P�b�g�̍쐬
    m_Socket.Create();
    //�\�P�b�g�̐ڑ�
    m_Socket.Connect(m_IP);

    //�X���b�h�̊J�n
    m_hRecieveThread = (HANDLE)_beginthreadex(
        NULL,
        0,
        RecieveThread,
        this,
        0,
        NULL
    );
}

unsigned int __stdcall CTCPClient::RecieveThread(void * pData)
{
    CTCPClient* pClient = reinterpret_cast<CTCPClient*>(pData);
    while (true)
    {
        DataHeader header;
        char   data[2048];
        int    headerSize = pClient->m_Socket.Recieve<DataHeader>(header);
        int    dataSize   = pClient->m_Socket.Recieve(data, header.Size);

        if (dataSize <= 0)
        {
            break;
        }

        pClient->Recieve(header, reinterpret_cast<void*>(data), dataSize);
    }
    _endthreadex(NULL);
    return 0;
}

int CTCPClient::Send(const void * pData, int datalen)
{
    return m_Socket.Send(pData, datalen);
}
