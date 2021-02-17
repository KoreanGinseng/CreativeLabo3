#include "TCPMultiServer.h"

CTCPMultiServer::CTCPMultiServer(unsigned int multiCount, int portNo, bool bStart)
{
	m_Client.resize(multiCount);
	m_Accept.SetPortNo(portNo);
	if (bStart)
	{
		Start();
	}
}

CTCPMultiServer::~CTCPMultiServer(void)
{
	if (!m_Accept.IsStart())
	{
		return;
	}
    m_Accept.CloseSocket();
	int size = m_Client.size();
	for (int i = 0; i < size; i++)
	{
		if (!m_Client[i].bConnect)
		{
			continue;
		}
        m_Client[i].Socket.CloseSocket();
	}
	m_Client.clear();

	//�X���b�h�̒�~�ҋ@
	WaitForSingleObject(m_hAcceptThread, INFINITE);
	CloseHandle(m_hAcceptThread);
}

void CTCPMultiServer::Start(void)
{
	m_Accept.Start();
	//�N���C�A���g�̏���������
	std::fill(m_Client.begin(), m_Client.end(), ClientData());
	//�ڑ��ҋ@�X���b�h�̊J�n
	m_hAcceptThread =
		(HANDLE)_beginthreadex(
			NULL,
			0,
			AcceptThread,
			this,
			0,
			NULL
		);
}

unsigned int __stdcall CTCPMultiServer::RecieveThread(void * pData)
{
    CTCPMultiServer* pms = reinterpret_cast<CTCPMultiServer*>(pData);
    ClientData* pClient = nullptr;
    int size = pms->m_Client.size();
    for (int i = 0; i < size; i++)
    {
        if (pms->m_Client[i].bConnect)
        {
            continue;
        }
        pClient = &(pms->m_Client[i]);
        break;
    }
    pClient->bConnect = true;
    while (true)
    {
        DataHeader header;
        char   data[2048];
        int    headerSize = pClient->Socket.Recieve<DataHeader>(header);
        if (headerSize <= 0)
        {
            break;
        }
        int    dataSize   = pClient->Socket.Recieve(data, header.Size);

        if (dataSize <= 0)
        {
            break;
        }

        pms->Recieve(header, reinterpret_cast<void*>(data), dataSize);
    }
    pClient->bConnect = false;
    pms->DisConnect();
    _endthreadex(NULL);
    return 0;
}

unsigned int __stdcall CTCPMultiServer::AcceptThread(void * pData)
{
    CTCPMultiServer* pms = reinterpret_cast<CTCPMultiServer*>(pData);
    //�\�P�b�g�̍쐬
    pms->m_Accept.Create();
    //�\�P�b�g���o�C���h
    pms->m_Accept.Bind();
    //�ڑ��ҋ@��Ԃɂ���
    pms->m_Accept.Listen();

    while (true)
    {
        //�ڑ��҂�
        SOCKET* ts = pms->m_Accept.Accept();
        if (*ts == INVALID_SOCKET)
        {
            break;
        }
        int size = pms->m_Client.size();
        for (int i = 0; i < size; i++)
        {
            if (pms->m_Client[i].bConnect)
            {
                continue;
            }
            //��M�X���b�h�̊J�n
            pms->m_Client[i].Socket.SetSocket(ts);
            pms->Connect();
            HANDLE hRecvThread =
                (HANDLE)_beginthreadex(
                    NULL,
                    0,
                    RecieveThread,
                    pms,
                    0,
                    NULL
                );
            break;
        }
    }
    _endthreadex(NULL);
    return 0;
}
