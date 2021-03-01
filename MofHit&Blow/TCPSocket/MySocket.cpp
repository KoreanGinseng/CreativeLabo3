#include "MySocket.h"

// ********************************************************************************
/// <summary>
/// WSAのStartUp
/// </summary>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
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
/// WSAのCleanUp
/// </summary>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
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
/// コンストラクタ
/// </summary>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
CMySocket::CMySocket(void)
    : m_Socket{ 0 }
    , m_PortNo{ 18900 }
    , m_bStart{ false }
{
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
CMySocket::~CMySocket(void)
{
}

// ********************************************************************************
/// <summary>
/// 開始
/// </summary>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
void CMySocket::Start(void)
{
    m_bStart = true;
}

// ********************************************************************************
/// <summary>
/// アクセプト
/// </summary>
/// <returns>ソケットゥ</returns>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
SOCKET * CMySocket::Accept(void)
{
    //接続してきたアドレス情報
    SOCKADDR_IN ta;
    //アドレス構造体のサイズ
    int addrin_size = sizeof(SOCKADDR_IN);
    //接続待ち
    m_TellSocket = accept(m_Socket, (struct sockaddr*)&ta, &addrin_size);
    return &m_TellSocket;
}

// ********************************************************************************
/// <summary>
/// ソケットの作成
/// </summary>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Create(void)
{
    //ソケットの作成
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
/// ソケットをバインド
/// </summary>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Bind(void)
{
    //アドレス構造体
    struct sockaddr_in tmp_addr;
    memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
    //ネットワークのデータを設定
    tmp_addr.sin_family      = AF_INET;
    tmp_addr.sin_port        = htons(m_PortNo);
    tmp_addr.sin_addr.s_addr = ADDR_ANY;
    //アドレスとソケットをバインド
    if ((bind(m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr))) == SOCKET_ERROR)
    {
        m_Error = SOCKETERROR::ERROR_BIND;
        closesocket(m_Socket);
    }
}

// ********************************************************************************
/// <summary>
/// ソケットの接続
/// </summary>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Connect(const std::string& ip)
{
    //アドレス構造体
    struct sockaddr_in tmp_addr;
    memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
    //ネットワークのデータを設定
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
/// 接続待機状態にする
/// </summary>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
void CMySocket::Listen(void)
{
    if ((listen(m_Socket, SOMAXCONN)) == SOCKET_ERROR)
    {
        m_Error = SOCKETERROR::ERROR_LISTEN;
        closesocket(m_Socket);
    }
}
