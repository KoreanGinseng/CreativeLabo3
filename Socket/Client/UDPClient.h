#pragma once
#include "../Common/MySocket.h"

// ********************************************************************************
/// <summary>
/// UDP�N���C�A���g�N���X
/// </summary>
// ********************************************************************************
class CUDPClient
{
protected:

	CMySocket   m_Socket;         //! �\�P�b�g�N

	std::string m_IP;             //! �ڑ���IP�A�h���X

	HANDLE      m_hRecieveThread; //! ��M�X���b�h

	HANDLE      m_hConnectThread; //! �ڑ��X���b�h

public:


};