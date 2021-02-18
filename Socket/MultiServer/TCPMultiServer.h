#pragma once
#include "../Common/MySocket.h"
#include <vector>

// ********************************************************************************
/// <summary>
/// �N���C�A���g
/// </summary>
// ********************************************************************************
struct ClientData
{
	bool      bConnect{ false };
    CMySocket Socket;
};

// ********************************************************************************
/// <summary>
/// �����l�ڑ��T�[�o�[
/// </summary>
// ********************************************************************************
class CTCPMultiServer
{
protected:

    CMySocket				          m_Accept;
    std::vector<ClientData>           m_Client{ 10, ClientData() };
    std::vector<std::pair<int, bool>> m_IdList{ 10, std::pair<int, bool>(0, false) };
    HANDLE                            m_hAcceptThread;
    int                               m_OwnerId{ -1 };

public:

    // ********************************************************************************
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <param name="bStart">�ʐM�J�n�t���O</param>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    explicit CTCPMultiServer(unsigned int multiCount = 10, int portNo = 18900, bool bStart = false);

    // ********************************************************************************
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual ~CTCPMultiServer(void);

    // ********************************************************************************
    /// <summary>
    /// �J�n
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    void Start(void);

    // ********************************************************************************
    /// <summary>
    /// ���M
    /// </summary>
    /// <param name="pData">���M�f�[�^</param>
    /// <param name="datalen"></param>
    /// <param name="sendType"></param>
    /// <param name="ids"></param>
    /// <param name="idlen"></param>
    /// <returns></returns>
    /// <created>���̂���,2021/02/18</created>
    /// <changed>���̂���,2021/02/18</changed>
    // ********************************************************************************
    int Send(const void* pData, int datalen, const SENDTYPE& sendType = SENDTYPE_BROADCAST, int* ids = nullptr, int idlen = 0);

    // ********************************************************************************
    /// <summary>
    /// ��M�p�X���b�h
    /// </summary>
    /// <param name="pData">this</param>
    /// <returns>0 : ����I��, ����ȊO : �G���[</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    static unsigned int WINAPI RecieveThread(void* pData);

    // ********************************************************************************
    /// <summary>
    /// �ڑ��p�X���b�h
    /// </summary>
    /// <param name="pData">this</param>
    /// <returns>0 : ����I��, ����ȊO : �G���[</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    static unsigned int WINAPI AcceptThread(void* pData);

    // ********************************************************************************
    /// <summary>
    /// �G���[�̎擾
    /// </summary>
    /// <returns>�G���[�̎擾</returns>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    inline const SOCKETERROR GetError(void) const
    {
        return m_Accept.GetError();
    }

    // ********************************************************************************
    /// <summary>
    /// �ڑ����ꂽ�u�Ԃ�1�x�������s�����֐�
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Connect(void) {}

    // ********************************************************************************
    /// <summary>
    /// �ڑ����؂ꂽ�u�Ԃ�1�x�������s�����֐�
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void DisConnect(void) {}

    // ********************************************************************************
    /// <summary>
    /// ��M�f�[�^���D���ɂ���֐�
    /// </summary>
    /// <param name="header">�f�[�^�w�b�_</param>
    /// <param name="data">��M�f�[�^</param>
    /// <param name="datalen">��M�f�[�^�T�C�Y</param>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    virtual void Recieve(const DataHeader& header, const void* data, int datalen) {}

private:

    int SendBroadCast(const void* pData, int datalen);
    int SendOtherCast(const void* pData, int datalen, int sendid);
    int SendUniqueCast(const void* pData, int datalen, int id);
    int SendMultiCast(const void* pData, int datalen, int* ids, int idlen);
    int SendOwnerCast(const void* pData, int datalen);
};
