#pragma once
#include "SocketDefine.h"

// ********************************************************************************
/// <summary>
/// �\�P�b�g�ʐM�p�C���^�[�t�F�[�X
/// </summary>
// ********************************************************************************
template< class T >
class CMySocket
{
protected:

    SOCKET      m_Socket;                               //! �\�P�b�g
    SOCKETERROR m_Error{ SOCKETERROR::ERROR_NONE };     //! �\�P�b�g�G���[
    int         m_PortNo;                               //! �|�[�g�ԍ�
    bool        m_bStart;                               //! �J�n�t���O

public:

    CMySocket(void) = default;
    
    // ********************************************************************************
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual ~CMySocket(void)
    {
        if (g_WsaState != WSASTATE_CLEANUP)
        {
            g_WsaState = WSASTATE_CLEANUP;
            WSACleanup();
        }
    }

    // ********************************************************************************
    /// <summary>
    /// WSA������
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Init(void)
    {
        //WSAStartUp
        if (g_WsaState != WSASTATE_INIT)
        {
            g_WsaState = WSASTATE_INIT;
            WSAData wsaData;
            int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (ret != 0)
            {
                m_Error = SOCKETERROR::ERROR_INIT;
            }
        }
    }

    // ********************************************************************************
    /// <summary>
    /// �J�n
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Start(void)
    {
        Init();
        m_bStart = true;
    }

    // ********************************************************************************
    /// <summary>
    /// �J�n�t���O�̎擾
    /// </summary>
    /// <returns>true : ���쒆, false : ����O</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    const bool IsStart(void) const noexcept
    {
        return m_bStart;
    }

    // ********************************************************************************
    /// <summary>
    /// ���M
    /// </summary>
    /// <param name="data">���M�f�[�^</param>
    /// <returns>���M�f�[�^�T�C�Y</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual int Send(const T& data)
    {
        return send(m_Socket, (char*)&data, sizeof(T), 0);
    }

    // ********************************************************************************
    /// <summary>
    /// ��M�f�[�^���D���ɂ���֐�
    /// </summary>
    /// <param name="data">��M�f�[�^</param>
    /// <param name="size">��M�f�[�^�T�C�Y</param>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Recieve(const T& data, int size) = 0;

    // ********************************************************************************
    /// <summary>
    /// operator()
    /// </summary>
    /// <returns>true : ����, false : �ُ�</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    const bool operator()(void) const noexcept
    {
        return (m_Error == SOCKETERROR::ERROR_NONE);
    }

    // ********************************************************************************
    /// <summary>
    /// �G���[�̎擾
    /// </summary>
    /// <returns>SOCKETERROR</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    const SOCKETERROR GetError(void) const noexcept
    {
        return m_Error;
    }
};