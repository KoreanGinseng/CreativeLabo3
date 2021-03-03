#pragma once
#include "SocketDefine.h"

// ********************************************************************************
/// <summary>
/// �ʐM�v���g�R��
/// </summary>
// ********************************************************************************
enum class Protocol
{
	TCP,
	UDP,
};

// ********************************************************************************
/// <summary>
/// �\�P�b�g�ʐM�p�C���^�[�t�F�[�X
/// </summary>
// ********************************************************************************
class CMySocket
{
public:

	static constexpr int AddressInSize = sizeof(SOCKADDR_IN); //! �A�h���X�\���̂̃T�C�Y

protected:

    SOCKET             m_Socket;                              //! �\�P�b�g
    SOCKETERROR        m_Error{ SOCKETERROR::ERROR_NONE };    //! �\�P�b�g�G���[
    int                m_PortNo;                              //! �|�[�g�ԍ�
    bool               m_bStart;                              //! �J�n�t���O
    SOCKET             m_TellSocket;                          //! �󂯓���\�P�b�g
    int                m_Id;                                  //! �ڑ�ID
	SOCKADDR_IN        m_TellAddress;                         //! �ڑ����Ă����A�h���X���
	struct sockaddr_in m_MyAddress;                           //! �A�h���X�\����
	Protocol           m_Protocol;                            //! �v���g�R��

public:

    // ********************************************************************************
    /// <summary>
    /// WSA��StartUp
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    static void StartUp(void);

    // ********************************************************************************
    /// <summary>
    /// WSA��CleanUp
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    static void CleanUp(void);

    // ********************************************************************************
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    CMySocket(void);
    
    // ********************************************************************************
    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual ~CMySocket(void);

    // ********************************************************************************
    /// <summary>
    /// �J�n
    /// </summary>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    virtual void Start(void);

    // ********************************************************************************
    /// <summary>
    /// �A�N�Z�v�g
    /// </summary>
    /// <returns>�\�P�b�g�D</returns>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    virtual SOCKET* Accept(void);

    // ********************************************************************************
    /// <summary>
    /// �\�P�b�g�̍쐬
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    virtual void Create(Protocol prot);

    // ********************************************************************************
    /// <summary>
    /// �\�P�b�g���o�C���h
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    virtual void Bind(void);

    // ********************************************************************************
    /// <summary>
    /// �\�P�b�g�̐ڑ�
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    virtual void Connect(const std::string& ip);

    // ********************************************************************************
    /// <summary>
    /// �ڑ��ҋ@��Ԃɂ���
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    virtual void Listen(void);

    // ********************************************************************************
    /// <summary>
    /// �\�P�b�g�����
    /// </summary>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    inline void CloseSocket(void)
    {
        closesocket(m_Socket);
    }

	// ********************************************************************************
	/// <summary>
	/// �v���g�R���̐ݒ�
	/// </summary>
	/// <param name="prot">�v���g�R��</param>
	/// <created>���̂���,2021/03/03</created>
	/// <changed>���̂���,2021/03/03</changed>
	// ********************************************************************************
	inline void SetProtocol(Protocol prot)
	{
		m_Protocol = prot;
	}

	// ********************************************************************************
	/// <summary>
	/// �v���g�R���̎擾
	/// </summary>
	/// <returns>�v���g�R��</returns>
	/// <created>���̂���,2021/03/03</created>
	/// <changed>���̂���,2021/03/03</changed>
	// ********************************************************************************
	inline Protocol GetProtocol(void) const
	{
		return m_Protocol;
	}

    // ********************************************************************************
    /// <summary>
    /// ID�̐ݒ�
    /// </summary>
    /// <param name="id">id</param>
    /// <created>���̂���,2021/02/18</created>
    /// <changed>���̂���,2021/02/18</changed>
    // ********************************************************************************
    inline void SetId(int id)
    {
        m_Id = id;
    }

    // ********************************************************************************
    /// <summary>
    /// ID�̎擾
    /// </summary>
    /// <returns>id</returns>
    /// <created>���̂���,2021/02/18</created>
    /// <changed>���̂���,2021/02/18</changed>
    // ********************************************************************************
    inline int GetId(void) const
    {
        return m_Id;
    }

    // ********************************************************************************
    /// <summary>
    /// �\�P�b�g�̎擾
    /// </summary>
    /// <returns>�\�P�b�g</returns>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    inline const SOCKET& GetSocket(void) const
    {
        return m_Socket;
    }

    // ********************************************************************************
    /// <summary>
    /// �|�[�g�ԍ��̐ݒ�
    /// </summary>
    /// <param name="portNo">�|�[�g�ԍ�</param>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    inline void SetPortNo(int portNo) noexcept
    {
        m_PortNo = portNo;
    }

    // ********************************************************************************
    /// <summary>
    /// �|�[�g�ԍ��̎擾
    /// </summary>
    /// <returns>�|�[�g�ԍ�</returns>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    inline int GetPortNo(void) const noexcept
    {
        return m_PortNo;
    }

    // ********************************************************************************
    /// <summary>
    /// �J�n�t���O�̎擾
    /// </summary>
    /// <returns>true : ���쒆, false : ����O</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    inline const bool IsStart(void) const noexcept
    {
        return m_bStart;
    }

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>���̂���,2021/03/02</created>
	/// <changed>���̂���,2021/03/02</changed>
	// ********************************************************************************
	inline unsigned short GetRecivePortNo(void) const noexcept
	{
		return ntohs(m_TellAddress.sin_port);
	}

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>���̂���,2021/03/02</created>
	/// <changed>���̂���,2021/03/02</changed>
	// ********************************************************************************
	inline unsigned short GetBindPortNo(void) const noexcept
	{
		return ntohs(m_MyAddress.sin_port);
	}

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>���̂���,2021/03/02</created>
	/// <changed>���̂���,2021/03/02</changed>
	// ********************************************************************************
	inline const char* GetReciveIP(void) const noexcept
	{
		return inet_ntoa(m_TellAddress.sin_addr);
	}

	// ********************************************************************************
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	/// <created>���̂���,2021/03/02</created>
	/// <changed>���̂���,2021/03/02</changed>
	// ********************************************************************************
	inline const char* GetBindIP(void) const noexcept
	{
		return inet_ntoa(m_MyAddress.sin_addr);
	}

	// ********************************************************************************
	/// <summary>
	/// ���M
	/// </summary>
	/// <param name="pData">���M�f�[�^</param>
	/// <param name="datalen">���M�f�[�^�T�C�Y</param>
	/// <returns>���M�f�[�^�T�C�Y</returns>
	/// <created>���̂���,2021/02/16</created>
	/// <changed>���̂���,2021/02/18</changed>
	// ********************************************************************************
	inline int Send(const void* pData, int datalen)
    {
		if (m_Protocol != Protocol::TCP)
		{
			return SendTo(pData, datalen, (SOCKADDR*)&m_TellAddress);
		}
        return send(m_Socket, reinterpret_cast<const char*>(pData), datalen, 0);
    }

	// ********************************************************************************
	/// <summary>
	/// ���M
	/// </summary>
	/// <param name="pData">���M�f�[�^</param>
	/// <param name="datalen">���M�f�[�^�T�C�Y</param>
	/// <returns>���M�f�[�^�T�C�Y</returns>
	/// <created>���̂���,2021/02/16</created>
	/// <changed>���̂���,2021/02/18</changed>
	// ********************************************************************************
	inline int SendTo(const void* pData, int datalen, SOCKADDR* to)
	{
		int tosize = AddressInSize;
		return sendto(m_Socket, reinterpret_cast<const char*>(pData), datalen, 0, to, tosize);
	}

	// ********************************************************************************
	/// <summary>
	/// ��M
	/// </summary>
	/// <param name="outData">�����o���f�[�^��</param>
	/// <returns>��M�f�[�^�T�C�Y</returns>
	/// <created>���̂���,2021/02/16</created>
	/// <changed>���̂���,2021/02/17</changed>
	// ********************************************************************************
	template< class T >
	inline int Recieve(T& outData) const
	{
		T Data;
		if (m_Protocol != Protocol::TCP)
		{
			return RecieveFrom(outData, (SOCKADDR*)&m_TellAddress);
		}
        int size = recv(m_Socket, (char*)&Data, sizeof(T), 0);
        outData  = Data;
		return size;
	}
	
	// ********************************************************************************
	/// <summary>
	/// ��M
	/// </summary>
	/// <param name="outData">�����o���f�[�^��</param>
	/// <returns>��M�f�[�^�T�C�Y</returns>
	/// <created>���̂���,2021/02/16</created>
	/// <changed>���̂���,2021/02/17</changed>
	// ********************************************************************************
	template< class T >
	inline int RecieveFrom(T& outData, SOCKADDR* from) const
	{
		int fromsize = AddressInSize;
		T Data;
		int size = recvfrom(m_Socket, (char*)&Data, sizeof(T), 0, from, &fromsize);
		outData = Data;
		return size;
	}

    // ********************************************************************************
    /// <summary>
    /// ��M
    /// </summary>
	/// <param name="outData">�����o���f�[�^��</param>
    /// <param name="datalen">��M�f�[�^�T�C�Y</param>
	/// <returns>��M�f�[�^�T�C�Y</returns>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    inline int Recieve(char* outData, int datalen) const
    {
		if (m_Protocol != Protocol::TCP)
		{
			return RecieveFrom(outData, datalen, (SOCKADDR*)&m_TellAddress);
		}
        int size = recv(m_Socket, outData, datalen, 0);
        return size;
    }
	
	// ********************************************************************************
	/// <summary>
	/// ��M
	/// </summary>
	/// <param name="outData">�����o���f�[�^��</param>
	/// <param name="datalen">��M�f�[�^�T�C�Y</param>
	/// <returns>��M�f�[�^�T�C�Y</returns>
	/// <created>���̂���,2021/02/17</created>
	/// <changed>���̂���,2021/02/17</changed>
	// ********************************************************************************
	inline int RecieveFrom(char* outData, int datalen, SOCKADDR* from) const
	{
		int fromsize = AddressInSize;
		int size = recvfrom(m_Socket, outData, datalen, 0, from, &fromsize);
		return size;
	}

    // ********************************************************************************
    /// <summary>
    /// 
    /// </summary>
    /// <param name="socket"></param>
    /// <created>���̂���,2021/02/18</created>
    /// <changed>���̂���,2021/02/18</changed>
    // ********************************************************************************
    inline void SetSocket(SOCKET* socket) noexcept
    {
        m_Socket = *socket;
    }

    // ********************************************************************************
    /// <summary>
    /// operator()
    /// </summary>
    /// <returns>true : ����, false : �ُ�</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    inline const bool operator()(void) const noexcept
    {
        return (m_Error == SOCKETERROR::ERROR_NONE);
    }

    // ********************************************************************************
    /// <summary>
    /// �\�P�b�g�Ƃ��Ď擾
    /// </summary>
    /// <returns>�\�P�b�g</returns>
    /// <created>���̂���,2021/02/17</created>
    /// <changed>���̂���,2021/02/17</changed>
    // ********************************************************************************
    operator const SOCKET& (void) const noexcept
    {
        return m_Socket;
    }

    // ********************************************************************************
    /// <summary>
    /// �G���[�̎擾
    /// </summary>
    /// <returns>SOCKETERROR</returns>
    /// <created>���̂���,2021/02/16</created>
    /// <changed>���̂���,2021/02/16</changed>
    // ********************************************************************************
    inline const SOCKETERROR GetError(void) const noexcept
    {
        return m_Error;
    }
};