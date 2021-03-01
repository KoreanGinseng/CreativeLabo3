#include "TCPClient.h"

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="ip">接続先IPアドレス</param>
/// <param name="portNo">接続先ポート番号</param>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
CTCPClient::CTCPClient(const char * ip, int portNo)
{
    m_IP = ip;
    m_Socket.SetPortNo(portNo);
}

// ********************************************************************************
/// <summary>
/// デストラクタ
/// </summary>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
CTCPClient::~CTCPClient(void)
{
    m_Socket.CloseSocket();
    //スレッドの停止待機
    WaitForSingleObject(m_hRecieveThread, INFINITE);
    CloseHandle(m_hRecieveThread);
}

// ********************************************************************************
/// <summary>
/// 開始
/// </summary>
/// <param name="ip">接続先IPアドレス</param>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
void CTCPClient::Start(const char * ip)
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
void CTCPClient::Start(void)
{
    m_Socket.Start();
    //ソケットの作成
    m_Socket.Create();
    //ソケットの接続
    m_Socket.Connect(m_IP);

    //スレッドの開始
    m_hRecieveThread = (HANDLE)_beginthreadex(
        NULL,
        0,
        RecieveThread,
        this,
        0,
        NULL
    );
}

// ********************************************************************************
/// <summary>
/// 受信スレッド
/// </summary>
/// <param name="pData">this</param>
/// <returns>0 : 正常終了, それ以外 : 異常終了</returns>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
unsigned int __stdcall CTCPClient::RecieveThread(void * pData)
{
    CTCPClient* pClient = reinterpret_cast<CTCPClient*>(pData);
    while (true)
    {
        DataHeader header;
        char   data[2048];
        int    headerSize = pClient->m_Socket.Recieve<DataHeader>(header);
        if (headerSize <= 0)
        {
            break;
        }
        int    dataSize   = pClient->m_Socket.Recieve(data, header.Size);

        if (dataSize <= 0)
        {
            break;
        }

        if (header.Type == DATATYPE_SETID)
        {
            pClient->m_Socket.SetId(*(reinterpret_cast<int*>(data)));
        }

        pClient->Recieve(header, reinterpret_cast<void*>(data), dataSize);
    }
    _endthreadex(NULL);
    return 0;
}

// ********************************************************************************
/// <summary>
/// データの送信
/// </summary>
/// <param name="pData">送信データポインタ</param>
/// <param name="datalen">送信データのサイズ</param>
/// <created>いのうえ,2021/02/17</created>
/// <changed>いのうえ,2021/02/17</changed>
// ********************************************************************************
int CTCPClient::Send(const void * pData, int datalen)
{
    return m_Socket.Send(pData, datalen);
}
