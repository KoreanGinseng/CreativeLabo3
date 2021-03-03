#pragma once

#include	"ServerCommon.h"

namespace Sample {

	namespace SampleServer {
		/**
		 * @brief			�T�[�o�[���C���^�[�t�F�C�X
		 */
		class IServer {
		public:
			/**
			 * �ڑ����N���C�A���g
			 */
			struct ConnectClient : public ClientInformation {
				/** �A�h���X */
				SocketAddr	addr;
				/** �������[�� */
				RoomPtr		Room;
			};
			using ConnectClientPtr = std::shared_ptr< ConnectClient >;

			/**
			 * @brief		����
			 */
			virtual bool Create(const ServerConfig& config) = 0;

			/**
			 * @brief		�S���ɑ��M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		type			���M���b�Z�[�W���ʎq
			 * @param[in]		ReTime			�đ��܂ł̎���
			 */
			virtual void SendAll(const void* pData, uint32_t Size, uint32_t type, double ReTime = 0) = 0;

			/**
			 * @brief		���M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		tuid			���M�Ώۂt�h�c
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			virtual bool Send(const void* pData, uint32_t Size, uint32_t tuid, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		���M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		it				���M�Ώ�
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			virtual bool Send(const void* pData, uint32_t Size, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		���M
			 * @param[in]		room			���M���[��
			 * @param[in]		it				���M�Ώ�
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			virtual bool SendRoom(const RoomPtr& room, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		���M
			 * @param[in]		code			�G���[�R�[�h
			 * @param[in]		tuid			���M�Ώۂt�h�c
			 * @param[in]		sendtype		���M�^�C�v
			 */
			virtual bool SendError(ErrorCode code, uint32_t tuid, SendType sendtype) = 0;

			/**
			 * @brief		���M
			 * @param[in]		code			�G���[�R�[�h
			 * @param[in]		addr			���M�ΏۃA�h���X
			 * @param[in]		sendtype		���M�^�C�v
			 */
			virtual bool SendError(ErrorCode code, SocketAddr addr, SendType sendtype) = 0;

			/**
			 * @brief		����
			 */
			virtual void Run() = 0;

			/**
			 * @brief		�N���C�A���g�̒ǉ�
			 */
			virtual void AddClient(ConnectClientPtr ct) = 0;

			/**
			 * @brief		�N���C�A���g�̍폜
			 */
			virtual void DeleteClientImpl(ConnectClientPtr ct) = 0;

			/**
			 * @brief		�N���C�A���g�̍폜
			 */
			virtual void DeleteClient(void) = 0;

			/**
			 * @brief		�N���C�A���g�̍폜
			 */
			virtual void DeleteClient(uint32_t id) = 0;

			/**
			 * @brief		���[���̒ǉ�
			 */
			virtual void AddRoom(RoomPtr ct) = 0;

			/**
			 * @brief		���[���̍폜
			 */
			virtual void DeleteRoom(void) = 0;

			/**
			 * @brief		���[���̍폜
			 */
			virtual void DeleteRoom(uint32_t id) = 0;

			/**
			 * @brief		�N���C�A���g�̃��[������
			 */
			virtual void EnterRoom(RoomPtr room, ConnectClientPtr ct) = 0;

			/**
			 * @brief		�N���C�A���g�̃��[���ޏo
			 */
			virtual void LeaveRoom(ConnectClientPtr ct) = 0;

			/**
			 * @brief		���
			 */
			virtual void Release() = 0;

			virtual bool IsClient(const uint32_t uid) = 0;
			virtual bool IsRoom(const uint32_t uid) = 0;
			virtual ConnectClientPtr GetClient(const uint32_t uid) = 0;
			virtual ConnectClientPtr GetClient(const SocketAddr& addr) = 0;
			virtual RoomPtr GetRoom(const uint32_t uid) = 0;
			virtual bool IsEnable() const = 0;
		protected:
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			virtual void ParseMessage(char* pData, RUDPMessage::Header& header, SocketAddr& addr) = 0;
			/**
			 * @brief		�ڑ����b�Z�[�W
			 */
			virtual void ParseConnectMessage(ConnectClientPtr client, ConnectMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) = 0;
			/**
			 * @brief		�N���C�A���g���X�g�v�����b�Z�[�W
			 */
			virtual void ParseClientListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) = 0;
			/**
			 * @brief		���[���쐬���b�Z�[�W
			 */
			virtual void ParseRoomCreateMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) = 0;
			/**
			 * @brief		���[���쐬���b�Z�[�W
			 */
			virtual void ParseRandomRoomMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) = 0;
			/**
			 * @brief		���[�����X�g�v�����b�Z�[�W
			 */
			virtual void ParseRoomListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) = 0;
			/**
			 * @brief		���̑��̃��b�Z�[�W
			 */
			virtual void ParseOtherMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) = 0;
		};
		//�|�C���^�u������
		using ServerPtr = std::shared_ptr<IServer>;

		/**
		 * @brief			�T�[�o�[
		 */
		class Server : public IServer {
		public:
		protected:
			/** �T�[�o�[�ݒ� */
			ServerConfig					config_;
			/** UDP�\�P�b�g */
			UdpSocketPtr					socket_;
			/** RUDP */
			RUDPMessage						RUDP_;
			/** �ڑ����N���C�A���g */
			std::list< ConnectClientPtr >	clients_;
			/** ���[�� */
			std::list< RoomPtr >			rooms_;
			/** �N���C�A���g���b�N */
			std::mutex						clientlock_;
			/** ���[�����b�N */
			std::mutex						roomlock_;
			/** �T�[�o�[�X���b�h */
			std::thread						thread_;
			/** ����t���O */
			bool							bEnabled;
			/** �������� */
			std::mt19937					mtRandom;
		public:
			/**
			 * @brief		�R���X�g���N�^
			 */
			Server();

			/**
			 * @brief		�f�X�g���N�^
			 */
			virtual ~Server();

			/**
			 * @brief		����
			 */
			bool Create(const ServerConfig& config) override;

			/**
			 * @brief		�S���ɑ��M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		type			���M���b�Z�[�W���ʎq
			 * @param[in]		ReTime			�đ��܂ł̎���
			 */
			void SendAll(const void* pData, uint32_t Size, uint32_t type, double ReTime = 0) override;

			/**
			 * @brief		���M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		tuid			���M�Ώۂt�h�c
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			bool Send(const void* pData, uint32_t Size, uint32_t tuid, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		���M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		it				���M�Ώ�
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			bool Send(const void* pData, uint32_t Size, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		���M
			 * @param[in]		room			���M���[��
			 * @param[in]		it				���M�Ώ�
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			bool SendRoom(const RoomPtr& room, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		���M
			 * @param[in]		code			�G���[�R�[�h
			 * @param[in]		tuid			���M�Ώۂt�h�c
			 * @param[in]		sendtype		���M�^�C�v
			 */
			bool SendError(ErrorCode code, uint32_t tuid, SendType sendtype) override;

			/**
			 * @brief		���M
			 * @param[in]		code			�G���[�R�[�h
			 * @param[in]		addr			���M�ΏۃA�h���X
			 * @param[in]		sendtype		���M�^�C�v
			 */
			bool SendError(ErrorCode code, SocketAddr addr, SendType sendtype) override;

			/**
			 * @brief		����
			 */
			void Run() override;

			/**
			 * @brief		�N���C�A���g�̒ǉ�
			 */
			void AddClient(ConnectClientPtr ct) override;

			/**
			 * @brief		�N���C�A���g�̍폜
			 */
			void DeleteClientImpl(ConnectClientPtr ct) override;

			/**
			 * @brief		�N���C�A���g�̍폜
			 */
			void DeleteClient(void) override;

			/**
			 * @brief		�N���C�A���g�̍폜
			 */
			void DeleteClient(uint32_t id) override;

			/**
			 * @brief		���[���̒ǉ�
			 */
			void AddRoom(RoomPtr ct) override;

			/**
			 * @brief		���[���̍폜
			 */
			void DeleteRoom(void) override;

			/**
			 * @brief		���[���̍폜
			 */
			void DeleteRoom(uint32_t id) override;

			/**
			 * @brief		�N���C�A���g�̃��[������
			 */
			void EnterRoom(RoomPtr room, ConnectClientPtr ct) override;

			/**
			 * @brief		�N���C�A���g�̃��[���ޏo
			 */
			void LeaveRoom(ConnectClientPtr ct) override;

			/**
			 * @brief		���
			 */
			void Release() override;

			bool IsClient(const uint32_t uid) override;
			bool IsRoom(const uint32_t uid) override;
			ConnectClientPtr GetClient(const uint32_t uid) override;
			ConnectClientPtr GetClient(const SocketAddr& addr) override;
			RoomPtr GetRoom(const uint32_t uid) override;
			bool IsEnable() const override { return bEnabled; }
		protected:
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			void ParseMessage(char* pData, RUDPMessage::Header& header, SocketAddr& addr) override;
			/**
			 * @brief		�ڑ����b�Z�[�W
			 */
			void ParseConnectMessage(ConnectClientPtr client, ConnectMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) override;
			/**
			 * @brief		�N���C�A���g���X�g�v�����b�Z�[�W
			 */
			void ParseClientListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) override;
			/**
			 * @brief		���[���쐬���b�Z�[�W
			 */
			void ParseRoomCreateMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) override;
			/**
			 * @brief		���[���쐬���b�Z�[�W
			 */
			void ParseRandomRoomMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) override;
			/**
			 * @brief		���[�����X�g�v�����b�Z�[�W
			 */
			void ParseRoomListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) override;
			/**
			 * @brief		���̑��̃��b�Z�[�W
			 */
			void ParseOtherMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) override;
		};
	}
}
