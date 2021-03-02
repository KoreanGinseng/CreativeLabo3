#include "TCPClient.h"

// ********************************************************************************
/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="ip">�ڑ���IP�A�h���X</param>
/// <param name="portNo">�ڑ���|�[�g�ԍ�</param>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
CTCPClient::CTCPClient(const char * ip, int portNo)
{
    m_IP = ip;
    m_Socket.SetPortNo(portNo);
}

// ********************************************************************************
/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
CTCPClient::~CTCPClient(void)
{
    m_Socket.CloseSocket();
    //�X���b�h�̒�~�ҋ@
    WaitForSingleObject(m_hRecieveThread, INFINITE);
    CloseHandle(m_hRecieveThread);
}

// ********************************************************************************
/// <summary>
/// �J�n
/// </summary>
/// <param name="ip">�ڑ���IP�A�h���X</param>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
void CTCPClient::Start(const char * ip)
{
    m_IP = ip;
    Start();
}

// ********************************************************************************
/// <summary>
/// �J�n
/// </summary>
/// <created>���̂���,2021/02/16</created>
/// <changed>���̂���,2021/02/16</changed>
// ********************************************************************************
void CTCPClient::Start(void)
{
    m_Socket.Start();
    //�\�P�b�g�̍쐬
    m_Socket.Create();
    //�\�P�b�g�̐ڑ�
    m_Socket.Connect(m_IP);

    //�X���b�h�̊J�n
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
/// ��M�X���b�h
/// </summary>
/// <param name="pData">this</param>
/// <returns>0 : ����I��, ����ȊO : �ُ�I��</returns>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
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
/// �f�[�^�̑��M
/// </summary>
/// <param name="pData">���M�f�[�^�|�C���^</param>
/// <param name="datalen">���M�f�[�^�̃T�C�Y</param>
/// <created>���̂���,2021/02/17</created>
/// <changed>���̂���,2021/02/17</changed>
// ********************************************************************************
int CTCPClient::Send(const void * pData, int datalen)
{
    return m_Socket.Send(pData, datalen);
}