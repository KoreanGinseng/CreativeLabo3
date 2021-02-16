#include "TCPMultiServer.h"

CTCPMultiServer::CTCPMultiServer(unsigned int multiCount, int portNo, bool bStart)
{
	m_Client.resize(multiCount);
	MySocket::m_PortNo = portNo;
	if (bStart)
	{
		Start();
	}
}

CTCPMultiServer::~CTCPMultiServer(void)
{
	if (!MySocket::m_bStart)
	{
		return;
	}
	closesocket(MySocket::m_Socket);
	int size = m_Client.size();
	for (int i = 0; i < size; i++)
	{
		if (!m_Client[i].bConnect)
		{
			continue;
		}
		closesocket(m_Client[i].Socket);
	}
	m_Client.clear();

	//スレッドの停止待機
	WaitForSingleObject(m_hAcceptThread, INFINITE);
	CloseHandle(m_hAcceptThread);
}

void CTCPMultiServer::Start(void)
{
	MySocket::Start();
	//クライアントの情報を初期化
	std::fill(m_Client.begin(), m_Client.end(), 0);
	//接続待機スレッドの開始
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
