#include "MySocket.h"

// ********************************************************************************
/// <summary>
/// WSA��StartUp
/// </summary>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
void CMySocket::StartUp(void)
{
    //WSAStartUp
    if (g_WsaState != WSASTATE_INIT)
    {
        g_WsaState = WSASTATE_INIT;
        WSAData wsaData;
        int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (ret != 0)
        {
        }
    }
}

// ********************************************************************************
/// <summary>
/// WSA��CleanUp
/// </summary>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
void CMySocket::CleanUp(void)
{
    if (g_WsaState == WSASTATE_INIT)
    {
        g_WsaState = WSASTATE_CLEANUP;
        WSACleanup();
    }
}

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
CMySocket::CMySocket(void)
    : m_Socket{ 0 }
    , m_PortNo{ 18900 }
    , m_bStart{ false }
{
}

// ********************************************************************************
/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
CMySocket::~CMySocket(void)
{
}

// ********************************************************************************
/// <summary>
/// �J�n
/// </summary>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
void CMySocket::Start(void)
{
    m_bStart = true;
}

// ********************************************************************************
/// <summary>
/// �A�N�Z�v�g
/// </summary>
/// <returns>�\�P�b�g�D</returns>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
SOCKET * CMySocket::Accept(void)
{
    //�ڑ����Ă����A�h���X���
    SOCKADDR_IN ta;
    //�A�h���X�\���̂̃T�C�Y
    int addrin_size = sizeof(SOCKADDR_IN);
    //�ڑ��҂�
    m_TellSocket = accept(m_Socket, (struct sockaddr*)&ta, &addrin_size);
    return &m_TellSocket;
}

// ********************************************************************************
/// <summary>
/// �\�P�b�g�̍쐬
/// </summary>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Create(void)
{
    //�\�P�b�g�̍쐬
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_Socket == INVALID_SOCKET)
    {
        m_Error = SOCKETERROR::ERROR_CREATE;
        int err = WSAGetLastError();
        switch (err)
        {
        case 0:
        default:
            break;
        }
    }
}

// ********************************************************************************
/// <summary>
/// �\�P�b�g���o�C���h
/// </summary>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Bind(void)
{
    //�A�h���X�\����
    struct sockaddr_in tmp_addr;
    memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
    //�l�b�g���[�N�̃f�[�^��ݒ�
    tmp_addr.sin_family      = AF_INET;
    tmp_addr.sin_port        = htons(m_PortNo);
    tmp_addr.sin_addr.s_addr = ADDR_ANY;
    //�A�h���X�ƃ\�P�b�g���o�C���h
    if ((bind(m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr))) == SOCKET_ERROR)
    {
        m_Error = SOCKETERROR::ERROR_BIND;
        closesocket(m_Socket);
    }
}

// ********************************************************************************
/// <summary>
/// �\�P�b�g�̐ڑ�
/// </summary>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Connect(const std::string& ip)
{
    //�A�h���X�\����
    struct sockaddr_in tmp_addr;
    memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
    //�l�b�g���[�N�̃f�[�^��ݒ�
    tmp_addr.sin_family      = AF_INET;
    tmp_addr.sin_port        = htons(m_PortNo);
    tmp_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
        closesocket(m_Socket);
        m_Error = SOCKETERROR::ERROR_CONNECT;
    }
}

// ********************************************************************************
/// <summary>
/// �ڑ��ҋ@��Ԃɂ���
/// </summary>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Listen(void)
{
    if ((listen(m_Socket, SOMAXCONN)) == SOCKET_ERROR)
    {
        m_Error = SOCKETERROR::ERROR_LISTEN;
        closesocket(m_Socket);
    }
}
