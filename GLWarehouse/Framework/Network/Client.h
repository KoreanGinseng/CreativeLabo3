#pragma once

#include	"ServerCommon.h"

namespace Sample {

	namespace SampleServer {
		/**
		 * @brief			�N���C�A���g���C���^�[�t�F�C�X
		 */
		class IClient {
		public:
			/** ��M����*/
			using OnReceiveFunction = std::function< void(const ServerHeader&, void*, const RUDPMessage::Header&) >;
			using OnConnectFunction = std::function< void() >;
			using OnRoomCreateFunction = std::function< void() >;
			using OnErrorFunction = std::function< void(ErrorCode) >;
			using OnClientListFunction = std::function< void(std::vector< ClientListMessage >&) >;
			using OnRoomListFunction = std::function< void(std::vector< RoomListMessage >&) >;
			using OnEnterRoomFunction = std::function< void() >;
			using OnLeaveRoomFunction = std::function< void() >;
			using OnRoomAddClientFunction = std::function< void() >;
			using OnRoomDeleteClientFunction = std::function< void() >;
			using OnRoomInformationFunction = std::function< void(const RoomPtr&) >;
			class IClientEvent {
			public:
				virtual void OnReceive(const ServerHeader& sheader, void* pData, const RUDPMessage::Header& header) {}
				virtual void OnConnect() {}
				virtual void OnRoomCreate() {}
				virtual void OnError(ErrorCode code) {}
				virtual void OnClientList(std::vector< ClientListMessage >& list) {}
				virtual void OnRoomList(std::vector< RoomListMessage >& list) {}
				virtual void OnEnterRoom() {}
				virtual void OnLeaveRoom() {}
				virtual void OnRoomAddClient() {}
				virtual void OnRoomDeleteClient() {}
				virtual void OnRoomInformation(const RoomPtr& room) {}
			};
			struct ClientEventFunctions {
				/**
				 * @brief		�ʃC�x���g�o�^
				 *				������eventInterface�̓o�^������ꍇ�����炪�D�悳���
				 */
				OnReceiveFunction			OnReceive;
				OnConnectFunction			OnConnect;
				OnRoomCreateFunction		OnRoomCreate;
				OnErrorFunction				OnError;
				OnClientListFunction		OnClientList;
				OnRoomListFunction			OnRoomList;
				OnEnterRoomFunction			OnEnterRoom;
				OnLeaveRoomFunction			OnLeaveRoom;
				OnRoomAddClientFunction		OnRoomAddClient;
				OnRoomDeleteClientFunction	OnRoomDeleteClient;
				OnRoomInformationFunction	OnRoomInformation;
				/**
				 * @brief		�C�x���g��M�p�C���^�[�t�F�C�X�̓o�^
				 */
				IClientEvent*				eventInterface;

				ClientEventFunctions()
				: OnReceive()
				, OnConnect()
				, OnRoomCreate()
				, OnError()
				, OnClientList()
				, OnRoomList()
				, OnEnterRoom()
				, OnLeaveRoom()
				, OnRoomAddClient()
				, OnRoomDeleteClient()
				, OnRoomInformation()
				, eventInterface(nullptr){
				}
			};

			/**
			 * @brief		����
			 * @param[in]		serverip		�T�[�o�[�̂h�o�A�h���X
			 * @param[in]		serverport		�T�[�o�[�̃|�[�g
			 * @param[in]		name			�ڑ���
			 * @param[in]		port			�ʐM�|�[�g
			 * @param[in]		recv			��M�֐�
			 */
			virtual bool Create(const ServerConfig& sconfig, const char* name, uint16_t port, ClientEventFunctions func) = 0;

			/**
			 * @brief		���M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		type			���M���b�Z�[�W���ʎq
			 * @param[in]		target			���M�Ώ�
			 * @param[in]		tuid			���M�Ώۂt�h�c
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			virtual bool Send(const void* pData, uint32_t Size, uint32_t type, SendTarget target, uint32_t tuid, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		�N���C�A���g���X�g�v��
			 */
			virtual void RequestClientList() = 0;

			/**
			 * @brief		���[�����X�g�v��
			 */
			virtual void RequestRoomList() = 0;

			/**
			 * @brief		���[���쐬�v��
			 * @param[in]		name			���[����
			 * @param[in]		maxclient		�ő�ڑ��l��
			 */
			virtual void CreateRoom(const char* name, uint32_t maxclient) = 0;

			/**
			 * @brief		���[���쐬�v��
			 * @param[in]		name			���[����
			 * @param[in]		maxclient		�ő�ڑ��l��
			 */
			virtual void RandomRoom(const char* name, uint32_t maxclient) = 0;

			/**
			 * @brief		���[�������v��
			 * @param[in]		uid				�������郋�[���h�c
			 */
			virtual void EnterRoom(uint32_t uid) = 0;

			/**
			 * @brief		���[���ޏo�v��
			 */
			virtual void LeaveRoom() = 0;

			/**
			 * @brief		����
			 */
			virtual void Run() = 0;

			/**
			 * @brief		���
			 */
			virtual void Release() = 0;

			/**
			 * @brief		���O�擾
			 */
			virtual const char* GetName() const = 0;

			/**
			 * @brief		UID�擾
			 */
			virtual const uint32_t GetUID() const = 0;

			/**
			 * @brief		�������[���̎擾
			 */
			virtual const RoomPtr& GetRoom() const = 0;

			/**
			 * @brief		�L������
			 */
			virtual bool IsEnable() const = 0;
		protected:
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			virtual void ParseMessage(char* pData, RUDPMessage::Header& header) = 0;
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			virtual void ParseClientListMessage(char* pData, RUDPMessage::Header& header) = 0;
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			virtual void ParseRoomMessage(char* pData, RUDPMessage::Header& header, RoomPtr room) = 0;
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			virtual void ParseRoomListMessage(char* pData, RUDPMessage::Header& header) = 0;
		};
		//�|�C���^�u������
		using ClientPtr = std::shared_ptr<IClient>;

		/**
		 * @brief			�T�[�o�[
		 */
		class Client : public IClient {
		public:
		protected:
			/** ��� */
			ClientInformation			info_;
			/** �������[�� */
			RoomPtr						room_;
			/** UDP�\�P�b�g */
			UdpSocketPtr				socket_;
			/** �T�[�o�[�A�h���X */
			SocketAddr					serverAddr_;
			/** RUDP */
			RUDPMessage					RUDP_;
			/** ��M����*/
			ClientEventFunctions		OnFunc;
		public:
			/**
			 * @brief		�R���X�g���N�^
			 */
			Client();

			/**
			 * @brief		�f�X�g���N�^
			 */
			virtual ~Client();

			/**
			 * @brief		����
			 * @param[in]		serverip		�T�[�o�[�̂h�o�A�h���X
			 * @param[in]		serverport		�T�[�o�[�̃|�[�g
			 * @param[in]		name			�ڑ���
			 * @param[in]		port			�ʐM�|�[�g
			 * @param[in]		recv			��M�֐�
			 */
			virtual bool Create(const ServerConfig& sconfig, const char* name, uint16_t port, ClientEventFunctions func) override;

			/**
			 * @brief		���M
			 * @param[in]		pData			���M�o�b�t�@
			 * @param[in]		Size			���M�o�b�t�@�T�C�Y
			 * @param[in]		type			���M���b�Z�[�W���ʎq
			 * @param[in]		target			���M�Ώ�
			 * @param[in]		tuid			���M�Ώۂt�h�c
			 * @param[in]		messagetype		���b�Z�\�W�^�C�v
			 * @param[in]		sendtype		���M�^�C�v
			 */
			bool Send(const void* pData, uint32_t Size, uint32_t type, SendTarget target, uint32_t tuid, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		�N���C�A���g���X�g�v��
			 */
			void RequestClientList() override;

			/**
			 * @brief		���[�����X�g�v��
			 */
			void RequestRoomList() override;

			/**
			 * @brief		���[���쐬�v��
			 * @param[in]		name			���[����
			 * @param[in]		maxclient		�ő�ڑ��l��
			 */
			void CreateRoom(const char* name, uint32_t maxclient) override;

			/**
			 * @brief		���[���쐬�v��
			 */
			void RandomRoom(const char* name, uint32_t maxclient) override;

			/**
			 * @brief		���[�������v��
			 * @param[in]		uid				�������郋�[���h�c
			 */
			void EnterRoom(uint32_t uid) override;

			/**
			 * @brief		���[���ޏo�v��
			 */
			void LeaveRoom() override;

			/**
			 * @brief		����
			 */
			void Run() override;

			/**
			 * @brief		���
			 */
			void Release() override;

			/**
			 * @brief		���O�擾
			 */
			const char* GetName() const override { return info_.name; }

			/**
			 * @brief		UID�擾
			 */
			const uint32_t GetUID() const override { return info_.uid; }

			/**
			 * @brief		�������[���̎擾
			 */
			const RoomPtr& GetRoom() const override { return room_; }

			/**
			 * @brief		�L������
			 */
			bool IsEnable() const override { return info_.uid > 0; }
		protected:
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			void ParseMessage(char* pData, RUDPMessage::Header& header) override;
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			void ParseClientListMessage(char* pData, RUDPMessage::Header& header) override;
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			void ParseRoomMessage(char* pData, RUDPMessage::Header& header, RoomPtr room) override;
			/**
			 * @brief		��M���b�Z�[�W�̃p�[�X
			 */
			void ParseRoomListMessage(char* pData, RUDPMessage::Header& header) override;
		};
	}
}
