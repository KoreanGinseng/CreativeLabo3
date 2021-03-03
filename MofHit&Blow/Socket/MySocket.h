#pragma once
#include "SocketDefine.h"

// ********************************************************************************
/// <summary>
/// 通信プロトコル
/// </summary>
// ********************************************************************************
enum class Protocol
{
	TCP,
	UDP,
};

// ********************************************************************************
/// <summary>
/// ソケット通信用インターフェース
/// </summary>
// ********************************************************************************
class CMySocket
{
public:

	static constexpr int AddressInSize = sizeof(SOCKADDR_IN); //! アドレス構造体のサイズ

protected:

    SOCKET             m_Socket;                              //! ソケット
    SOCKETERROR        m_Error{ SOCKETERROR::ERROR_NONE };    //! ソケットエラー
    int                m_PortNo;                              //! ポート番号
    bool               m_bStart;                              //! 開始フラグ
    SOCKET             m_TellSocket;                          //! 受け入れソケット
    int                m_Id;                                  //! 接続ID
	SOCKADDR_IN        m_TellAddress;                         //! 接続してきたアドレス情報
	struct sockaddr_in m_MyAddress;                           //! アドレス構造体
	Protocol           m_Protocol;                            //! プロトコル

public:

    // ********************************************************************************
    /// <summary>
    /// WSAのStartUp
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    static void StartUp(void);

    // ********************************************************************************
    /// <summary>
    /// WSAのCleanUp
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    static void CleanUp(void);

    // ********************************************************************************
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    CMySocket(void);
    
    // ********************************************************************************
    /// <summary>
    /// デストラクタ
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual ~CMySocket(void);

    // ********************************************************************************
    /// <summary>
    /// 開始
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void Start(void);

    // ********************************************************************************
    /// <summary>
    /// アクセプト
    /// </summary>
    /// <returns>ソケットゥ</returns>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    virtual SOCKET* Accept(void);

    // ********************************************************************************
    /// <summary>
    /// ソケットの作成
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    virtual void Create(Protocol prot);

    // ********************************************************************************
    /// <summary>
    /// ソケットをバインド
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    virtual void Bind(void);

    // ********************************************************************************
    /// <summary>
    /// ソケットの接続
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    virtual void Connect(const std::string& ip);

    // ********************************************************************************
    /// <summary>
    /// 接続待機状態にする
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    virtual void Listen(void);

    // ********************************************************************************
    /// <summary>
    /// ソケットを閉じる
    /// </summary>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    inline void CloseSocket(void)
    {
        closesocket(m_Socket);
    }

	// ********************************************************************************
	/// <summary>
	/// プロトコルの設定
	/// </summary>
	/// <param name="prot">プロトコル</param>
	/// <created>いのうえ,2021/03/03</created>
	/// <changed>いのうえ,2021/03/03</changed>
	// ********************************************************************************
	inline void SetProtocol(Protocol prot)
	{
		m_Protocol = prot;
	}

	// ********************************************************************************
	/// <summary>
	/// プロトコルの取得
	/// </summary>
	/// <returns>プロトコル</returns>
	/// <created>いのうえ,2021/03/03</created>
	/// <changed>いのうえ,2021/03/03</changed>
	// ********************************************************************************
	inline Protocol GetProtocol(void) const
	{
		return m_Protocol;
	}

    // ********************************************************************************
    /// <summary>
    /// IDの設定
    /// </summary>
    /// <param name="id">id</param>
    /// <created>いのうえ,2021/02/18</created>
    /// <changed>いのうえ,2021/02/18</changed>
    // ********************************************************************************
    inline void SetId(int id)
    {
        m_Id = id;
    }

    // ********************************************************************************
    /// <summary>
    /// IDの取得
    /// </summary>
    /// <returns>id</returns>
    /// <created>いのうえ,2021/02/18</created>
    /// <changed>いのうえ,2021/02/18</changed>
    // ********************************************************************************
    inline int GetId(void) const
    {
        return m_Id;
    }

    // ********************************************************************************
    /// <summary>
    /// ソケットの取得
    /// </summary>
    /// <returns>ソケット</returns>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    inline const SOCKET& GetSocket(void) const
    {
        return m_Socket;
    }

    // ********************************************************************************
    /// <summary>
    /// ポート番号の設定
    /// </summary>
    /// <param name="portNo">ポート番号</param>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    inline void SetPortNo(int portNo) noexcept
    {
        m_PortNo = portNo;
    }

    // ********************************************************************************
    /// <summary>
    /// ポート番号の取得
    /// </summary>
    /// <returns>ポート番号</returns>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    inline int GetPortNo(void) const noexcept
    {
        return m_PortNo;
    }

    // ********************************************************************************
    /// <summary>
    /// 開始フラグの取得
    /// </summary>
    /// <returns>true : 動作中, false : 動作前</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    inline const bool IsStart(void) const noexcept
    {
        return m_bStart;
    }

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>いのうえ,2021/03/02</created>
	/// <changed>いのうえ,2021/03/02</changed>
	// ********************************************************************************
	inline unsigned short GetRecivePortNo(void) const noexcept
	{
		return ntohs(m_TellAddress.sin_port);
	}

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>いのうえ,2021/03/02</created>
	/// <changed>いのうえ,2021/03/02</changed>
	// ********************************************************************************
	inline unsigned short GetBindPortNo(void) const noexcept
	{
		return ntohs(m_MyAddress.sin_port);
	}

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>いのうえ,2021/03/02</created>
	/// <changed>いのうえ,2021/03/02</changed>
	// ********************************************************************************
	inline const char* GetReciveIP(void) const noexcept
	{
		return inet_ntoa(m_TellAddress.sin_addr);
	}

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>いのうえ,2021/03/02</created>
	/// <changed>いのうえ,2021/03/02</changed>
	// ********************************************************************************
	inline const char* GetBindIP(void) const noexcept
	{
		return inet_ntoa(m_MyAddress.sin_addr);
	}

	// ********************************************************************************
	/// <summary>
	/// 送信
	/// </summary>
	/// <param name="pData">送信データ</param>
	/// <param name="datalen">送信データサイズ</param>
	/// <returns>送信データサイズ</returns>
	/// <created>いのうえ,2021/02/16</created>
	/// <changed>いのうえ,2021/02/18</changed>
	// ********************************************************************************
	inline int Send(const void* pData, int datalen)
    {
		if (m_Protocol != Protocol::TCP)
		{
			return SendTo(pData, datalen, (SOCKADDR*)&m_TellAddress);
		}
        return send(m_Socket, reinterpret_cast<const char*>(pData), datalen, 0);
    }

	// ********************************************************************************
	/// <summary>
	/// 送信
	/// </summary>
	/// <param name="pData">送信データ</param>
	/// <param name="datalen">送信データサイズ</param>
	/// <returns>送信データサイズ</returns>
	/// <created>いのうえ,2021/02/16</created>
	/// <changed>いのうえ,2021/02/18</changed>
	// ********************************************************************************
	inline int SendTo(const void* pData, int datalen, SOCKADDR* to)
	{
		int tosize = AddressInSize;
		return sendto(m_Socket, reinterpret_cast<const char*>(pData), datalen, 0, to, tosize);
	}

	// ********************************************************************************
	/// <summary>
	/// 受信
	/// </summary>
	/// <param name="outData">書き出しデータ先</param>
	/// <returns>受信データサイズ</returns>
	/// <created>いのうえ,2021/02/16</created>
	/// <changed>いのうえ,2021/02/17</changed>
	// ********************************************************************************
	template< class T >
	inline int Recieve(T& outData) const
	{
		T Data;
		if (m_Protocol != Protocol::TCP)
		{
			return RecieveFrom(outData, (SOCKADDR*)&m_TellAddress);
		}
        int size = recv(m_Socket, (char*)&Data, sizeof(T), 0);
        outData  = Data;
		return size;
	}
	
	// ********************************************************************************
	/// <summary>
	/// 受信
	/// </summary>
	/// <param name="outData">書き出しデータ先</param>
	/// <returns>受信データサイズ</returns>
	/// <created>いのうえ,2021/02/16</created>
	/// <changed>いのうえ,2021/02/17</changed>
	// ********************************************************************************
	template< class T >
	inline int RecieveFrom(T& outData, SOCKADDR* from) const
	{
		int fromsize = AddressInSize;
		T Data;
		int size = recvfrom(m_Socket, (char*)&Data, sizeof(T), 0, from, &fromsize);
		outData = Data;
		return size;
	}

    // ********************************************************************************
    /// <summary>
    /// 受信
    /// </summary>
	/// <param name="outData">書き出しデータ先</param>
    /// <param name="datalen">受信データサイズ</param>
	/// <returns>受信データサイズ</returns>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    inline int Recieve(char* outData, int datalen) const
    {
		if (m_Protocol != Protocol::TCP)
		{
			return RecieveFrom(outData, datalen, (SOCKADDR*)&m_TellAddress);
		}
        int size = recv(m_Socket, outData, datalen, 0);
        return size;
    }
	
	// ********************************************************************************
	/// <summary>
	/// 受信
	/// </summary>
	/// <param name="outData">書き出しデータ先</param>
	/// <param name="datalen">受信データサイズ</param>
	/// <returns>受信データサイズ</returns>
	/// <created>いのうえ,2021/02/17</created>
	/// <changed>いのうえ,2021/02/17</changed>
	// ********************************************************************************
	inline int RecieveFrom(char* outData, int datalen, SOCKADDR* from) const
	{
		int fromsize = AddressInSize;
		int size = recvfrom(m_Socket, outData, datalen, 0, from, &fromsize);
		return size;
	}

    // ********************************************************************************
    /// <summary>
    /// 
    /// </summary>
    /// <param name="socket"></param>
    /// <created>いのうえ,2021/02/18</created>
    /// <changed>いのうえ,2021/02/18</changed>
    // ********************************************************************************
    inline void SetSocket(SOCKET* socket) noexcept
    {
        m_Socket = *socket;
    }

    // ********************************************************************************
    /// <summary>
    /// operator()
    /// </summary>
    /// <returns>true : 正常, false : 異常</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    inline const bool operator()(void) const noexcept
    {
        return (m_Error == SOCKETERROR::ERROR_NONE);
    }

    // ********************************************************************************
    /// <summary>
    /// ソケットとして取得
    /// </summary>
    /// <returns>ソケット</returns>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    operator const SOCKET& (void) const noexcept
    {
        return m_Socket;
    }

    // ********************************************************************************
    /// <summary>
    /// エラーの取得
    /// </summary>
    /// <returns>SOCKETERROR</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    inline const SOCKETERROR GetError(void) const noexcept
    {
        return m_Error;
    }
};