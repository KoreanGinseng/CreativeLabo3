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

    std::string m_IP; //! 接続先IPアドレス

public:

	// ********************************************************************************
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="ip">接続先IPアドレス</param>
	/// <param name="portNo">接続先ポート番号</param>
	/// <created>いのうえ,2021/02/16</created>
	/// <changed>いのうえ,2021/02/16</changed>
	// ********************************************************************************
	CTCPClient(const char* ip = "", int portNo = 18900)
	{
        m_IP               = ip;
        MySocket::m_PortNo = portNo;
	}

	// ********************************************************************************
	/// <summary>
	/// デストラクタ
	/// </summary>
	/// <created>いのうえ,2021/02/16</created>
	/// <changed>いのうえ,2021/02/16</changed>
	// ********************************************************************************
	virtual ~CTCPClient(void)
	{
		closesocket(MySocket::m_Socket);
	}

    // ********************************************************************************
    /// <summary>
    /// 開始
    /// </summary>
    /// <param name="ip">接続先IPアドレス</param>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    void Start(const char* ip)
    {
        m_IP = ip;
        Start();
    }

    // ********************************************************************************
    /// <summary>
    /// 開始
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    void Start(void) override
    {
        MySocket::Start();
        //ソケットの作成
        MySocket::m_Socket = socket(AF_INET, SOCK_STREAM, 0);
        if (MySocket::m_Socket == INVALID_SOCKET)
        {
            MySocket::m_Error = SOCKETERROR::ERROR_CREATE;
        }

        //アドレス構造体
        struct sockaddr_in tmp_addr;
        memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
        //ネットワークのデータを設定
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
    /// 受信データを好きにする関数
    /// </summary>
    /// <param name="data">受信データ</param>
    /// <param name="size">受信データサイズ</param>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void Recieve(const T& data, int size) override {}
};

