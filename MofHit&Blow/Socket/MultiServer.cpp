#include "MultiServer.h"

// ********************************************************************************
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="bStart">通信開始フラグ</param>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
CMultiServer::CMultiServer(Protocol prot, unsigned int multiCount, int portNo, bool bStart)
{
	m_Client.resize(multiCount);
	m_IdList.resize(multiCount);
    int size = m_IdList.size();
    for (int i = 0; i < size; i++)
    {
        m_IdList[i].first = i;
    }
	m_Accept.SetPortNo(portNo);
	m_Accept.SetProtocol(prot);
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
CMultiServer::~CMultiServer(void)
{
	if (!m_Accept.IsStart())
	{
		return;
	}
    m_Accept.CloseSocket();
	int size = m_Client.size();
	for (int i = 0; i < size; i++)
	{
		if (!m_Client[i].bConnect)
		{
			continue;
		}
        m_Client[i].Socket.CloseSocket();
	}
	m_Client.clear();

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
void CMultiServer::Start(void)
{
	m_Accept.Start();
	//クライアントの情報を初期化
	std::fill(m_Client.begin(), m_Client.end(), ClientData());
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
/// 送信
/// </summary>
/// <param name="pData">送信データ</param>
/// <param name="datalen">データのサイズ</param>
/// <param name="sendType">データの種類</param>
/// <param name="ids">送り先ID</param>
/// <param name="idlen">送るIDの数</param>
/// <returns>送ったサイズ</returns>
/// <created>いのうえ,2021/02/18</created>
/// <changed>いのうえ,2021/02/18</changed>
// ********************************************************************************
int CMultiServer::Send(const void * pData, int datalen, const SENDTYPE & sendType, int * ids, int idlen)
{
    switch (sendType)
    {
    case SENDTYPE_BROADCAST:
        return SendBroadCast(pData, datalen);
    case SENDTYPE_OTHERCAST:
        return SendOtherCast(pData, datalen, ids[0]);
    case SENDTYPE_UNIQUECAST:
        return SendUniqueCast(pData, datalen, ids[0]);
    case SENDTYPE_MULTICAST:
        return SendMultiCast(pData, datalen, ids, idlen);
    case SENDTYPE_OWNERCAST:
        return SendOwnerCast(pData, datalen);
    }
    return 0;
}

// ********************************************************************************
/// <summary>
/// 受信用スレッド
/// </summary>
/// <param name="pData">this</param>
/// <returns>0 : 正常終了, それ以外 : エラー</returns>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
unsigned int __stdcall CMultiServer::RecieveThread(void * pData)
{
    CMultiServer* pms   = reinterpret_cast<CMultiServer*>(pData);
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
        DataHeader header;
        char   data[2048];
        int    headerSize = pClient->Socket.Recieve<DataHeader>(header);
        if (headerSize <= 0)
        {
            break;
        }
        int    dataSize   = pClient->Socket.Recieve(data, header.Size);

        if (dataSize <= 0)
        {
            break;
        }

        pms->Recieve(header, reinterpret_cast<void*>(data), dataSize);
    }
    pClient->bConnect = false;
    auto id           = std::find_if(
        pms->m_IdList.begin(), pms->m_IdList.end(),
        [&](const UseId_t& v) {return v.first == pClient->Socket.GetId(); }
    );
    id->second        = false;
    pms->DisConnect();
    _endthreadex(NULL);
    return 0;
}

// ********************************************************************************
/// <summary>
/// 接続用スレッド
/// </summary>
/// <param name="pData">this</param>
/// <returns>0 : 正常終了, それ以外 : エラー</returns>
/// <created>いのうえ,2021/02/16</created>
/// <changed>いのうえ,2021/02/16</changed>
// ********************************************************************************
unsigned int __stdcall CMultiServer::AcceptThread(void * pData)
{
    CMultiServer* pms = reinterpret_cast<CMultiServer*>(pData);
    //ソケットの作成
    pms->m_Accept.Create(pms->m_Accept.GetProtocol());
    //ソケットをバインド
    pms->m_Accept.Bind();
    //接続待機状態にする
    pms->m_Accept.Listen();

    while (true)
    {
        //接続待ち
        SOCKET* ts = pms->m_Accept.Accept();
        if (*ts == INVALID_SOCKET)
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
            pms->m_Client[i].Socket.SetSocket(ts);
            if (pms->m_OwnerId == -1)
            {
                pms->m_OwnerId = pms->m_Client[i].Socket.GetId();
            }
            auto id = std::find_if(pms->m_IdList.begin(), pms->m_IdList.end(), [](const UseId_t& v) {return v.second == false; });
            id->second = true;
            pms->m_Client[i].Socket.SetId(id->first);
            struct IDData : public DataHeader
            {
                int id;
            } data;
            data.Type = DATATYPE_SETID;
            data.id   = id->first;
            data.Size = sizeof(data.id);
            pms->m_Client[i].Socket.Send(&data, sizeof(data));
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

int CMultiServer::SendBroadCast(const void * pData, int datalen)
{
    int s    = 0;
    int size = m_Client.size();
    for (int i = 0; i < size; i++)
    {
        s += m_Client[i].Socket.Send(pData, datalen);
    }
    return s;
}

int CMultiServer::SendOtherCast(const void * pData, int datalen, int sendid)
{
    int s    = 0;
    int size = m_Client.size();
    for (int i = 0; i < size; i++)
    {
        if (m_Client[i].Socket.GetId() == sendid)
        {
            continue;
        }
        s += m_Client[i].Socket.Send(pData, datalen);
    }
    return s;
}

int CMultiServer::SendUniqueCast(const void * pData, int datalen, int id)
{
    auto v = std::find_if(m_Client.begin(), m_Client.end(), [&](const ClientData& v) {return v.Socket.GetId() == id; });
    int  s = v->Socket.Send(pData, datalen);
    return s;
}

int CMultiServer::SendMultiCast(const void * pData, int datalen, int * ids, int idlen)
{
    int s = 0;
    for (int i = 0; i < idlen; i++)
    {
        s += SendUniqueCast(pData, datalen, ids[i]);
    }
    return s;
}

int CMultiServer::SendOwnerCast(const void * pData, int datalen)
{
    int s = SendUniqueCast(pData, datalen, m_OwnerId);
    return s;
}
