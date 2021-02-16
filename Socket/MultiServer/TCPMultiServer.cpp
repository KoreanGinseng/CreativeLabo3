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

	//�X���b�h�̒�~�ҋ@
	WaitForSingleObject(m_hAcceptThread, INFINITE);
	CloseHandle(m_hAcceptThread);
}

void CTCPMultiServer::Start(void)
{
	MySocket::Start();
	//�N���C�A���g�̏���������
	std::fill(m_Client.begin(), m_Client.end(), 0);
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
