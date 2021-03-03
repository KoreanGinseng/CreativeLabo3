#include "MultiServer.h"

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="bStart">�ʐM�J�n�t���O</param>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
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
/// �f�X�g���N�^
/// </summary>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
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

	//�X���b�h�̒�~�ҋ@
	WaitForSingleObject(m_hAcceptThread, INFINITE);
	CloseHandle(m_hAcceptThread);
}

// ********************************************************************************
/// <summary>
/// �J�n
/// </summary>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
void CMultiServer::Start(void)
{
	m_Accept.Start();
	//�N���C�A���g�̏���������
	std::fill(m_Client.begin(), m_Client.end(), ClientData());
	//�ڑ��ҋ@�X���b�h�̊J�n
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
/// ���M
/// </summary>
/// <param name="pData">���M�f�[�^</param>
/// <param name="datalen">�f�[�^�̃T�C�Y</param>
/// <param name="sendType">�f�[�^�̎��</param>
/// <param name="ids">�����ID</param>
/// <param name="idlen">����ID�̐�</param>
/// <returns>�������T�C�Y</returns>
/// <created>���̂���,2021/02/18</created>
/// <changed>���̂���,2021/02/18</changed>
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
/// ��M�p�X���b�h
/// </summary>
/// <param name="pData">this</param>
/// <returns>0 : ����I��, ����ȊO : �G���[</returns>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
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
/// �ڑ��p�X���b�h
/// </summary>
/// <param name="pData">this</param>
/// <returns>0 : ����I��, ����ȊO : �G���[</returns>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
unsigned int __stdcall CMultiServer::AcceptThread(void * pData)
{
    CMultiServer* pms = reinterpret_cast<CMultiServer*>(pData);
    //�\�P�b�g�̍쐬
    pms->m_Accept.Create(pms->m_Accept.GetProtocol());
    //�\�P�b�g���o�C���h
    pms->m_Accept.Bind();
    //�ڑ��ҋ@��Ԃɂ���
    pms->m_Accept.Listen();

    while (true)
    {
        //�ڑ��҂�
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
            //��M�X���b�h�̊J�n
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
