#pragma once
#include "../Common/SocketDefine.h"

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
template < class T >
class CTCPClient
{
private:

	SOCKET      m_Socket;

	SOCKETERROR m_Error;

public:

	CTCPClient(const char* ip)
	{
		//WSAStartUp
		WSAData wsaData;
		int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			m_Error = SOCKETERROR::ERROR_INIT;
		}

		//ソケットの作成
		m_Socket = socket(AF_INET, SOCK_STREAM, 0);
		if (m_Socket == INVALID_SOCKET)
		{
			m_Error = SOCKETERROR::ERROR_CREATE;
		}

		//アドレス構造体
		struct sockaddr_in tmp_addr;
		memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
		//ネットワークのデータを設定
		tmp_addr.sin_family = AF_INET;
		tmp_addr.sin_port = htons(18900);
		tmp_addr.sin_addr.s_addr = inet_addr(ip);
		if (connect(m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			closesocket(m_Socket);
			m_Error = SOCKETERROR::ERROR_CONNECT;
		}
	}

	virtual ~CTCPClient(void)
	{
		closesocket(m_Socket);
		WSACleanup();
	}

	const int Send(T data) const
	{
		return send(m_Socket, (char*)&data, sizeof(T), 0);
	}
};

