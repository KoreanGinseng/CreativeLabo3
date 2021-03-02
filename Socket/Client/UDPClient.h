#pragma once
#include "../Common/MySocket.h"

// ********************************************************************************
/// <summary>
/// UDPクライアントクラス
/// </summary>
// ********************************************************************************
class CUDPClient
{
protected:

	CMySocket   m_Socket;         //! ソケット君

	std::string m_IP;             //! 接続先IPアドレス

	HANDLE      m_hRecieveThread; //! 受信スレッド

	HANDLE      m_hConnectThread; //! 接続スレッド

public:


};