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
CClient::CClient(const char * ip, int portNo)
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
CClient::~CClient(void)
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
void CClient::Start(const char * ip, Protocol prot)
{
    m_IP = ip;
    Start(prot);
}

// ********************************************************************************
/// <summary>
/// 開始
/// </summary>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
void CClient::Start(Protocol prot)
{
	m_Protocol = prot;
    m_Socket.Start();
    //ソケットの作成
    m_Socket.Create(prot);

	//接続スレッドの開始
	m_hConnectThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		ConnectThread,
		this,
		0,
		NULL
	);
}

// ********************************************************************************
/// <summary>
/// 接続スレッド
/// </summary>
/// <param name="pData">this</param>
/// <returns>0 : 正常終了, それ以外 : 異常終了</returns>
/// <created>いのうえ,2021/03/02</created>
/// <changed>いのうえ,2021/03/02</changed>
// ********************************************************************************
unsigned int __stdcall CClient::ConnectThread(void * pData)
{
	CClient* pClient = reinterpret_cast<CClient*>(pData);
	//ソケットの接続
	pClient->m_Socket.Connect(pClient->m_IP);

	//スレッドの開始
	pClient->m_hRecieveThread = (HANDLE)_beginthreadex(
		NULL,
		0,
		RecieveThread,
		pClient,
		0,
		NULL
	);
	_endthreadex(NULL);
	return 0;
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
unsigned int __stdcall CClient::RecieveThread(void * pData)
{
    CClient* pClient = reinterpret_cast<CClient*>(pData);
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
int CClient::Send(const void * pData, int datalen)
{
    return m_Socket.Send(pData, datalen);
}
