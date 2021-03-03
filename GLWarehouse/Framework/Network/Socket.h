#pragma once

#include	"../Common.h"

namespace Sample {

	/**
	 * @brief			�\�P�b�g�֘A�u������
	 */
	using SocketDescriptor = SOCKET;
	using SocketAddr = sockaddr_in;

	/**
	 * @brief			�\�P�b�g���C���^�[�t�F�C�X
	 */
	class ISocket {
	public:
		/**
		 * @brief		����
		 */
		virtual bool Create(void) = 0;

		/**
		 * @brief		�\�P�b�g�����
		 */
		virtual bool Close(void) = 0;

		virtual SocketDescriptor Socket(void) = 0;
	};

	/**
	 * @brief			UDP�\�P�b�g���C���^�[�t�F�C�X
	 */
	class IUdpSocket : public ISocket {
	public:
		/**
		 * @brief			�\�P�b�g�̃o�C���h
		 * @param[in]		port			�|�[�g�ԍ�
		 */
		virtual bool Bind(const uint16_t port) = 0;
		/**
		 * @brief			�\�P�b�g�̃o�C���h
		 * @param[in]		addr			���M�A�h���X
		 */
		virtual bool Bind(const SocketAddr& addr) = 0;

		/**
		 * @brief			�f�[�^�̑��M
		 * @param[in]		addr			���M�A�h���X
		 * @param[in]		pBuf			���M�o�b�t�@
		 * @param[in]		size			���M�o�b�t�@�T�C�Y
		 */
		virtual uint32_t Send(const SocketAddr& addr, const void* pBuf, const uint32_t size) = 0;
		/**
		 * @brief			�f�[�^�̑��M
		 * @param[in]		addr			���M�A�h���X
		 * @param[in]		port			���M�|�[�g
		 * @param[in]		pBuf			���M�o�b�t�@
		 * @param[in]		size			���M�o�b�t�@�T�C�Y
		 */
		virtual uint32_t Send(uint32_t addr, uint16_t port, const void* pBuf, const uint32_t size) = 0;

		/**
		 * @brief			�f�[�^�̎�M
		 * @param[in]		pBuf			��M�o�b�t�@
		 * @param[in]		size			��M�o�b�t�@�T�C�Y
		 * @param[out]		addr			��M�A�h���X
		 */
		virtual uint32_t Receive(void* pBuf, const uint32_t size, SocketAddr& addr) = 0;
		/**
		 * @brief			�f�[�^�̎�M
		 * @param[in]		pBuf			��M�o�b�t�@
		 * @param[in]		size			��M�o�b�t�@�T�C�Y
		 * @param[in]		bKeep			��M�o�b�t�@�ۑ��t���O
		 * @param[out]		addr			��M�A�h���X
		 */
		virtual uint32_t Receive(void* pBuf, const uint32_t size, bool bKeep, SocketAddr& addr) = 0;

		/**
		 * @brief		��M�f�[�^�����邩���f
		 */
		virtual bool IsReceive(void) = 0;
	};
	//�|�C���^�u������
	using UdpSocketPtr = std::shared_ptr<IUdpSocket>;

	/**
	 * @brief			UDP�\�P�b�g
	 */
	class UdpSocket : public IUdpSocket {
	protected:
		/** �\�P�b�g */
		SocketDescriptor	Socket_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		UdpSocket();

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~UdpSocket();

		/**
		 * @brief		����
		 */
		bool Create(void) override;

		/**
		 * @brief			�\�P�b�g�̃o�C���h
		 * @param[in]		port			�|�[�g�ԍ�
		 */
		bool Bind(const uint16_t port) override;
		/**
		 * @brief			�\�P�b�g�̃o�C���h
		 * @param[in]		addr			���M�A�h���X
		 */
		bool Bind(const SocketAddr& addr) override;

		/**
		 * @brief			�f�[�^�̑��M
		 * @param[in]		addr			���M�A�h���X
		 * @param[in]		pBuf			���M�o�b�t�@
		 * @param[in]		size			���M�o�b�t�@�T�C�Y
		 */
		uint32_t Send(const SocketAddr& addr, const void* pBuf, const uint32_t size) override;
		/**
		 * @brief			�f�[�^�̑��M
		 * @param[in]		addr			���M�A�h���X
		 * @param[in]		port			���M�|�[�g
		 * @param[in]		pBuf			���M�o�b�t�@
		 * @param[in]		size			���M�o�b�t�@�T�C�Y
		 */
		uint32_t Send(uint32_t addr, uint16_t port, const void* pBuf, const uint32_t size) override;

		/**
		 * @brief			�f�[�^�̎�M
		 * @param[in]		pBuf			��M�o�b�t�@
		 * @param[in]		size			��M�o�b�t�@�T�C�Y
		 * @param[out]		addr			��M�A�h���X
		 */
		uint32_t Receive(void* pBuf, const uint32_t size, SocketAddr& addr) override;
		/**
		 * @brief			�f�[�^�̎�M
		 * @param[in]		pBuf			��M�o�b�t�@
		 * @param[in]		size			��M�o�b�t�@�T�C�Y
		 * @param[in]		bKeep			��M�o�b�t�@�ۑ��t���O
		 * @param[out]		addr			��M�A�h���X
		 */
		uint32_t Receive(void* pBuf, const uint32_t size, bool bKeep, SocketAddr& addr) override;

		/**
		 * @brief		�\�P�b�g�����
		 */
		bool Close(void) override;

		/**
		 * @brief		��M�f�[�^�����邩���f
		 */
		bool IsReceive(void) override;

		virtual SocketDescriptor Socket(void) { return Socket_; }
	};
}
