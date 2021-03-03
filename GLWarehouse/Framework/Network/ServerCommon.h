#pragma once

#include	"RUDPMessage.h"

namespace Sample {

	namespace SampleServer {
		/** ���O�̍ő啶���� */
		constexpr int ServerNameMax = 32;

		/** �ʐM�o�b�t�@�T�C�Y */
		constexpr uint32_t BufferMax = 8192;

		/** RUDP�đ����� */
		constexpr double RUDPRetryTime = 0.15;

		/** �ڑ��ێ����M���� */
		constexpr uint32_t KeepAliveTime = 10;

		/** �T�[�o�[���ł̍폜������� */
		constexpr uint32_t ServerDeleteTime = 60;

		/** �T�[�o�[�������b�Z�[�W */
		constexpr uint32_t ServerMessage = 0xFA000000;

		/** �G���[�R�[�h */
		enum class ErrorCode : unsigned int {
			/** �T�[�o�[���󂯎��ׂ����b�Z�[�W����Ȃ����̂����M���ꂽ */
			NotServerMessage,
			/** �s���ȃN���C�A���g */
			InvalidClient,
			/** �Q�[���h�c���Ⴄ */
			IncorrectGameID,
			/** �o�[�W�����h�c���Ⴄ */
			IncorrectVersionID,
			/** ���[�����Ȃ� */
			InvalidRoom,
			/** ���Ƀ��[�����ɂ��� */
			AlreadyInTheRoom,
			/** ���[���ɓ����Ă��Ȃ� */
			NotInTheRoom,
			/** ���[���ɍő�l�������ς� */
			MaxClientInTheRoom,
		};

		/** ���M�Ώ� */
		enum class SendTarget : unsigned char {
			/** �T�[�o�[�ւ̃��b�Z�[�W */
			Server,
			/** �S�ẴN���C�A���g�ւ̃��b�Z�[�W */
			AllClient,
			/** ���[�����̃N���C�A���g�ւ̃��b�Z�[�W */
			RoomClient,
			/** ����N���C�A���g�ւ̃��b�Z�[�W */
			ClientID,
		};

		/** ���b�Z�[�W��� */
		enum class MessageType {
			/** �N���C�A���g�ڑ����b�Z�[�W */
			Connect,
			/** �N���C�A���g�ؒf���b�Z�[�W */
			DisConnect,
			/** �N���C�A���g���X�g�v�����b�Z�[�W */
			ClientList,
			/** ���[���쐬�v�����b�Z�[�W */
			CreateRoom,
			/** �����_���ȕ����ւ̓��������[���쐬�v�����b�Z�[�W */
			CreateAndEnterRandomRoom,
			/** ���[�����X�g�v�����b�Z�[�W */
			RoomList,
			/** ���[�������v�����b�Z�[�W */
			EnterRoom,
			/** ���[���ޏo�v�����b�Z�[�W */
			LeaveRoom,
			/** ���[�����v�����b�Z�[�W */
			RoomInformation,
			/** ���[���̃N���C�A���g�����������b�Z�[�W */
			RoomAddClient,
			/** ���[���̃N���C�A���g�����������b�Z�[�W */
			RoomDeleteClient,
			/** �G���[���b�Z�[�W */
			Error,
			/** �ڑ��������b�Z�[�W */
			KeepAlive,
			/** ���̑��̃Q�[�������b�Z�[�W */
			Other,
		};

		/** ���b�Z�[�W��� */
		enum class SendType : unsigned char {
			/** UDP */
			Udp,
			/** RUDP���� */
			RUdp,
		};

		/**
		 * �T�[�o�[�R���t�B�O
		 */
		struct ServerConfig {
			/** �ڑ��h�o */
			std::string IP;
			/** �ڑ��|�[�g */
			uint16_t port;
			/** �Q�[���h�c */
			uint32_t	gameID;
			/** �o�[�W�����h�c */
			uint32_t	versionID;
			
			ServerConfig()
			: IP()
			, port(0)
			, gameID(0)
			, versionID(0)
			{
			}
			ServerConfig(const char* ip, uint16_t port, uint32_t gid, uint32_t vid)
			: IP(ip)
			, port(port)
			, gameID(gid)
			, versionID(vid)
			{
			}
		};

		/**
		 * �ڑ����N���C�A���g
		 */
		struct ClientInformation {
			/** ���j�[�N�h�c */
			uint32_t	uid;
			/** ���O */
			char		name[ServerNameMax];
			/** �ŏI�ʐM���� */
			double		lastTime;

			ClientInformation()
			: uid(0)
			, name()
			, lastTime(0)
			{
			}
		};

		/**
		 * ���[�����
		 */
		struct RoomData {
		};

		/**
		 * ���[�����
		 */
		struct Room {
			/** ���j�[�N�h�c */
			uint32_t							uid;
			/** ���O */
			char								name[ServerNameMax];
			/** �ő�l�� */
			uint32_t							maxClient;
			/** �ڑ����N���C�A���gID */
			std::vector< ClientInformation >	clients;
			/** ���[���o�^��� */
			std::vector< RoomData >				roomData;
		};
		using RoomPtr = std::shared_ptr< Room >;

#pragma pack(push, 1)
		/**
		 * ���b�Z�[�W�w�b�_�[
		 */
		struct ServerHeader {
			/** ���M�҂̃��j�[�N�h�c */
			uint32_t	senduid;
			/**
			 * ���M��̃��j�[�N�h�c
			 * target��ClientID�̏ꍇ�̂ݗL��
			 */
			uint32_t	recvuid;
			/** ���M�^�[�Q�b�g */
			SendTarget	target;
			/** ���b�Z�[�W�^�C�v */
			MessageType	type;
			/** ���M�^�C�v */
			SendType	sendType;
		};

		/**
		 * �ڑ����b�Z�[�W
		 */
		struct ConnectMessage {
			/** �ڑ��҂̖��O */
			char		name[ServerNameMax];
			/** �Q�[���h�c */
			uint32_t	gameID;
			/** �o�[�W�����h�c */
			uint32_t	versionID;
		};

		/**
		 * �ڑ����N���C�A���g
		 */
		struct ClientListMessage {
			/** ���j�[�N�h�c */
			uint32_t	uid;
			/** ���O */
			char		name[ServerNameMax];
		};

		/**
		 * ���[���쐬
		 */
		struct RoomCreateMessage {
			/** ���O */
			char		name[ServerNameMax];
			/** �ő�l�� */
			uint32_t	maxClient;
		};

		/**
		 * ���[��
		 */
		struct RoomListMessage {
			/** ���j�[�N�h�c */
			uint32_t	uid;
			/** ���O */
			char		name[ServerNameMax];
		};

		/**
		 * �G���[���b�Z�[�W
		 */
		struct ErrorMessage {
			/** �G���[�R�[�h */
			ErrorCode	code;
		};
#pragma pack(pop)

#ifdef OUTPUTLOG
#define		SERVER_LOGLEVEL			3

#if SERVER_LOGLEVEL >= 1
#define		SERVER_INFO_LOG_LV1(log)		std::cout << log << std::endl;
#else
#define		SERVER_INFO_LOG_LV1(log)
#endif // SERVER_LOGLEVEL >= 1

#if SERVER_LOGLEVEL >= 2
#define		SERVER_INFO_LOG_LV2(log)		std::cout << log << std::endl;
#else
#define		SERVER_INFO_LOG_LV2(log)
#endif // SERVER_LOGLEVEL >= 2

#if SERVER_LOGLEVEL >= 3
#define		SERVER_INFO_LOG_LV3(log)		std::cout << log << std::endl;
#else
#define		SERVER_INFO_LOG_LV3(log)
#endif // SERVER_LOGLEVEL >= 3

#else
#define		SERVER_INFO_LOG_LV1(log)
#define		SERVER_INFO_LOG_LV2(log)
#define		SERVER_INFO_LOG_LV3(log)
#endif
	}

}