#pragma once
#include "../Common/MySocket.h"

// ********************************************************************************
/// <summary>
/// �N���C�A���g
/// </summary>
// ********************************************************************************
struct ClientData
{
    bool   bConnect;
    SOCKET Socket;
};

// ********************************************************************************
/// <summary>
/// �����l�ڑ��T�[�o�[
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
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="bStart">�ʐM�J�n�t���O</param>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
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
    /// �f�X�g���N�^
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
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
    void Start(void)
    {
        MySocket::Start();
        //�N���C�A���g�̏���������
        memset(m_Client, 0, sizeof(m_Client));
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
    /// ��M�p�X���b�h
    /// </summary>
    /// <param name="pData">data</param>
    /// <returns>0 : ����I��, ����ȊO : �G���[</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
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
    /// �ڑ��p�X���b�h
    /// </summary>
    /// <param name="pData">data</param>
    /// <returns>0 : ����I��, ����ȊO : �G���[</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    static unsigned int WINAPI AcceptThread(void* pData)
    {
        CTCPMultiServer* pms = reinterpret_cast<CTCPMultiServer*>(pData);
        //�\�P�b�g�̍쐬
        pms->m_Socket = socket(AF_INET, SOCK_STREAM, 0);
        if (pms->m_Socket == INVALID_SOCKET)
        {
            pms->m_Error = SOCKETERROR::ERROR_CREATE;
        }
        //�A�h���X�\����
        struct sockaddr_in tmp_addr;
        memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
        //�l�b�g���[�N�̃f�[�^��ݒ�
        tmp_addr.sin_family      = AF_INET;
        tmp_addr.sin_port        = htons(pms->m_PortNo);
        tmp_addr.sin_addr.s_addr = ADDR_ANY;
        //�A�h���X�ƃ\�P�b�g���o�C���h
        if ((bind(pms->m_Socket, (struct sockaddr*)&tmp_addr, sizeof(struct sockaddr))) == SOCKET_ERROR)
        {
            pms->m_Error = SOCKETERROR::ERROR_BIND;
            closesocket(pms->m_Socket);
        }
        //�ڑ��ҋ@��Ԃɂ���
        if ((listen(pms->m_Socket, SOMAXCONN)) == SOCKET_ERROR)
        {
            pms->m_Error = SOCKETERROR::ERROR_LISTEN;
            closesocket(pms->m_Socket);
        }

        while (true)
        {
            //�ڑ����Ă����A�h���X���
            SOCKADDR_IN ta;
            //�A�h���X�\���̂̃T�C�Y
            int addrin_size = sizeof(SOCKADDR_IN);
            //�ڑ��҂�
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
                //��M�X���b�h�̊J�n
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
    /// �ڑ����ꂽ�u�Ԃ�1�x�������s�����֐�
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Connect(void) {};

    // ********************************************************************************
    /// <summary>
    /// �ڑ����؂ꂽ�u�Ԃ�1�x�������s�����֐�
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void DisConnect(void) {};

    // ********************************************************************************
    /// <summary>
    /// ��M�f�[�^���D���ɂ���֐�
    /// </summary>
    /// <param name="data">��M�f�[�^</param>
    /// <param name="size">��M�f�[�^�T�C�Y</param>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Recieve(const T& data, int size) override {}
};

