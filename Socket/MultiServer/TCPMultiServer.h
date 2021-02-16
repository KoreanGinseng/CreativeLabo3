#pragma once
#include "../Common/MySocket.h"

// ********************************************************************************
/// <summary>
/// クライアント
/// </summary>
// ********************************************************************************
struct ClientData
{
    bool   bConnect;
    SOCKET Socket;
};

// ********************************************************************************
/// <summary>
/// 複数人接続サーバー
/// </summary>
// ********************************************************************************
template< class T, int N = 10 >
class CTCPMultiServer : public CMySocket< T >
{
protected:

    using MySocket = CMySocket<T>;
    
    ClientData  m_Client[N];
    HANDLE      m_hAcceptThread;

public:

    // ********************************************************************************
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="bStart">通信開始フラグ</param>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    explicit CTCPMultiServer(int portNo = 18900, bool bStart = false)
    {
        MySocket::m_PortNo = portNo;
        if (bStart)
        {
            Start();
        }
    }

    // ********************************************************************************
    /// <summary>
    /// デストラクタ
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual ~CTCPMultiServer(void)
    {
        if (!MySocket::m_bStart)
        {
            return;
        }
        closesocket(MySocket::m_Socket);
        for (int i = 0; i < N; i++)
        {
            if (!m_Client[i].bConnect)
            {
                continue;
            }
            closesocket(m_Client[i].Socket);
        }
        //スレッドの停止待機
        WaitForSingleObject(m_hAcceptThread, INFINITE);
        CloseHandle(m_hAcceptThread);
    }

    // ********************************************************************************
    /// <summary>
    /// 開始
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    void Start(void)
    {
        MySocket::Start();
        //クライアントの情報を初期化
        memset(m_Client, 0, sizeof(m_Client));
        //接続待機スレッドの開始
        m_hAcceptThread = 
            (HANDLE)_beginthreadex(
                NULL,
                0,
                AcceptThread,
                this,
                0,
                NULL
            );
    }

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
        for (int i = 0; i < N; i++)
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
            T Data;
            int size = recv(pClient->Socket, (char*)&Data, sizeof(T), 0);
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
            for (int i = 0; i < N; i++)
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

