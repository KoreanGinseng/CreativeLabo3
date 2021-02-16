#pragma once
#include "../Common/MySocket.h"

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
template < class T >
class CTCPClient : public CMySocket< T >
{
protected:

    using MySocket = CMySocket<T>;

    std::string m_IP; //! �ڑ���IP�A�h���X

public:

	// ********************************************************************************
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="ip">�ڑ���IP�A�h���X</param>
	/// <param name="portNo">�ڑ���|�[�g�ԍ�</param>
	/// <created>���̂���,2021/02/16</created>
	/// <changed>���̂���,2021/02/16</changed>
	// ********************************************************************************
	CTCPClient(const char* ip = "", int portNo = 18900)
	{
        m_IP               = ip;
        MySocket::m_PortNo = portNo;
	}

	// ********************************************************************************
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	/// <created>���̂���,2021/02/16</created>
	/// <changed>���̂���,2021/02/16</changed>
	// ********************************************************************************
	virtual ~CTCPClient(void)
	{
		closesocket(MySocket::m_Socket);
	}

    // ********************************************************************************
    /// <summary>
    /// �J�n
    /// </summary>
    /// <param name="ip">�ڑ���IP�A�h���X</param>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    void Start(const char* ip)
    {
        m_IP = ip;
        Start();
    }

    // ********************************************************************************
    /// <summary>
    /// �J�n
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    void Start(void) override
    {
        MySocket::Start();
        //�\�P�b�g�̍쐬
        MySocket::m_Socket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket::m_Socket == INVALID_SOCKET)
        {
            MySocket::m_Error = SOCKETERROR::ERROR_CREATE;
        }

        //�A�h���X�\����
        struct sockaddr_in tmp_addr;
        memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
        //�l�b�g���[�N�̃f�[�^��ݒ�
        tmp_addr.sin_family      = AF_INET;
        tmp_addr.sin_port        = htons(MySocket::m_PortNo);
        tmp_addr.sin_addr.s_addr = inet_addr(m_IP.c_str());
        if (connect(MySocket::m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
        {
            closesocket(MySocket::m_Socket);
            MySocket::m_Error = SOCKETERROR::ERROR_CONNECT;
        }
    }

    // ********************************************************************************
    /// <summary>
    /// ��M�f�[�^���D���ɂ���֐�
    /// </summary>
    /// <param name="data">��M�f�[�^</param>
    /// <param name="size">��M�f�[�^�T�C�Y</param>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Recieve(const T& data, int size) override {}
};

