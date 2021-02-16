#pragma once
#include "../Common/MySocket.h"
#include <vector>

// ********************************************************************************
/// <summary>
/// クライアント
/// </summary>
// ********************************************************************************
struct ClientData
{
	bool   bConnect{ false };
	SOCKET Socket  { 0 };
};

// ********************************************************************************
/// <summary>
/// 複数人接続サーバー
/// </summary>
// ********************************************************************************
class CTCPMultiServer : public 
{
protected:

    using MySocket = CMySocket;
	CMySocket				 m_Accept;
	std::vector<ClientData>  m_Client{ 10, ClientData() };
    HANDLE                   m_hAcceptThread;

	struct Header {
		int Type;
		int Size;
	};
public:

    // ********************************************************************************
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="bStart">通信開始フラグ</param>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
	explicit CTCPMultiServer(unsigned int multiCount = 10, int portNo = 18900, bool bStart = false);

    // ********************************************************************************
    /// <summary>
    /// デストラクタ
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
	virtual ~CTCPMultiServer(void);

    // ********************************************************************************
    /// <summary>
    /// 開始
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
	void Start(void);

    // ********************************************************************************
    /// <summary>
    /// 受信用スレッド
    /// </summary>
    /// <param name="pData">data</param>
    /// <returns>0 : 正常終了, それ以外 : エラー</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    static unsigned int WINAPI RecieveThread(void* pData)
    {
        CTCPMultiServer* pms = reinterpret_cast<CTCPMultiServer*>(pData);
        ClientData* pClient = nullptr;
		int size = pms->m_Client.size();
        for (int i = 0; i < size; i++)
        {
            if (pms->m_Client[i].bConnect)
            {
                continue;
            }
            pClient = &(pms->m_Client[i]);
            break;
        }
        pClient->bConnect = true;
        while (true)
        {
			Header Data;
            int size = recv(pClient->Socket, (char*)&Data, sizeof(Data), 0);

			if (h->Type == 0)
			{
				return pClient->Socket.Receiver<int>();
			}
            pms->Recieve(Data, size);
            if (size <= 0)
            {
                break;
            }
        }
        pClient->bConnect = false;
        pms->DisConnect();
        _endthreadex(NULL);
        return 0;
    }

    // ********************************************************************************
    /// <summary>
    /// 接続用スレッド
    /// </summary>
    /// <param name="pData">data</param>
    /// <returns>0 : 正常終了, それ以外 : エラー</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    static unsigned int WINAPI AcceptThread(void* pData)
    {
        CTCPMultiServer* pms = reinterpret_cast<CTCPMultiServer*>(pData);
        //ソケットの作成
        pms->m_Socket = socket(AF_INET, SOCK_STREAM, 0);
        if (pms->m_Socket == INVALID_SOCKET)
        {
            pms->m_Error = SOCKETERROR::ERROR_CREATE;
        }
        //アドレス構造体
        struct sockaddr_in tmp_addr;
        memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
        //ネットワークのデータを設定
        tmp_addr.sin_family      = AF_INET;
        tmp_addr.sin_port        = htons(pms->m_PortNo);
        tmp_addr.sin_addr.s_addr = ADDR_ANY;
        //アドレスとソケットをバインド
        if ((bind(pms->m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr))) == SOCKET_ERROR)
        {
            pms->m_Error = SOCKETERROR::ERROR_BIND;
            closesocket(pms->m_Socket);
        }
        //接続待機状態にする
        if ((listen(pms->m_Socket, SOMAXCONN)) == SOCKET_ERROR)
        {
            pms->m_Error = SOCKETERROR::ERROR_LISTEN;
            closesocket(pms->m_Socket);
        }

        while (true)
        {
            //接続してきたアドレス情報
            SOCKADDR_IN ta;
            //アドレス構造体のサイズ
            int addrin_size = sizeof(SOCKADDR_IN);
            //接続待ち
            SOCKET ts = accept(pms->m_Socket, (struct sockaddr*)&ta, &addrin_size);
            if (ts == INVALID_SOCKET)
            {
                break;
            }
			int size = pms->m_Client.size();
            for (int i = 0; i < size; i++)
            {
                if (pms->m_Client[i].bConnect)
                {
                    continue;
                }
                //受信スレッドの開始
                pms->m_Client[i].Socket = ts;
                pms->Connect();
                HANDLE hRecvThread = 
                    (HANDLE)_beginthreadex(
                        NULL,
                        0,
                        RecieveThread,
                        pms,
                        0,
                        NULL
                    );
                break;
            }
        }
        _endthreadex(NULL);
        return 0;
    }

    // ********************************************************************************
    /// <summary>
    /// 接続された瞬間に1度だけ実行される関数
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void Connect(void) {};

    // ********************************************************************************
    /// <summary>
    /// 接続が切れた瞬間に1度だけ実行される関数
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void DisConnect(void) {};

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
