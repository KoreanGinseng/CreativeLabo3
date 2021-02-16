#pragma once
#include "../Common/SocketDefine.h"

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
template < class T >
class CTCPServer
{
private:

	SOCKET      m_Socket;
	SOCKET      m_Reciever;

	SOCKETERROR m_Error = SOCKETERROR::ERROR_NONE;

public:
	
	CTCPServer(void)
	{
		//WSAStartUp
		WSAData wsaData;
		int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			m_Error = SOCKETERROR::ERROR_INIT;
		}
		//�\�P�b�g�̍쐬
		m_Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_Socket == INVALID_SOCKET)
		{
			m_Error = SOCKETERROR::ERROR_CREATE;
		}
		//�A�h���X�\����
		struct sockaddr_in tmp_addr;
		memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
		//�l�b�g���[�N�̃f�[�^��ݒ�
		tmp_addr.sin_family = AF_INET;
		tmp_addr.sin_port = htons(18900);
		tmp_addr.sin_addr.s_addr = ADDR_ANY;
		//�A�h���X�ƃ\�P�b�g���o�C���h
		if ((bind(m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
		{
			closesocket(m_Socket);
			m_Error = SOCKETERROR::ERROR_BIND;
		}
		//�ڑ��ҋ@��Ԃɂ���
		if ((listen(m_Socket, SOMAXCONN)) == SOCKET_ERROR)
		{
			closesocket(m_Socket);
			m_Error = SOCKETERROR::ERROR_LISTEN;
		}
		//�ڑ����Ă����A�h���X���
		SOCKADDR_IN ta;
		//�A�h���X�\���̂̃T�C�Y
		int addrin_size = sizeof(SOCKADDR_IN);

		//�ڑ��҂�(Blocking)
		m_Reciever = accept(m_Socket, (struct sockaddr*)&ta, &addrin_size);
	}
	
	virtual ~CTCPServer(void)
	{
		closesocket(m_Socket);
		closesocket(m_Reciever);
		WSACleanup();
	}

	const SOCKETERROR operator()(void) const noexcept
	{
		return m_Error;
	}

	const T Recieve(void) const
	{
		T Data;
		T s = recv(m_Reciever, (char*)&Data, sizeof(T), 0);
		return Data;
	}
};

