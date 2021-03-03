#pragma once

#include	"Stage.h"
#include	"NetworkKeyMove.h"

namespace Sample {

	/**
	 * @brief		�Q�[�����C���V�[���̒�`
	 */
	class NetworkScene : public Scene, public SampleServer::IClient::IClientEvent {
	private:
		StagePtr						Stage_;
		std::list< NetworkKeyMovePtr >	Characters_;
		ActorPtr						Clear_;
		SampleServer::ServerConfig		config;
		SampleServer::ServerPtr			server;
		SampleServer::ClientPtr			client;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		NetworkScene();

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~NetworkScene();

		/**
		 * @brief		�p����̃V�[���̍\�z�������Ȃ�
		 */
		void OnCreate() override;

		/**
		 * @brief		�p����̃V�[���̍X�V�����������Ȃ�
		 */
		void OnUpdate() override;

		/**
		 * @brief		�p����̃V�[���̍X�V�㏈���������Ȃ�
		 */
		void OnLateUpdate() override;

		/** �l�b�g���[�N�n�̃C�x���g��` */
		void OnReceive(const SampleServer::ServerHeader& sheader, void* pData, const RUDPMessage::Header& header) override;
		void OnConnect() override;
		void OnRoomCreate() override;
		void OnError(SampleServer::ErrorCode code) override;
		void OnClientList(std::vector< SampleServer::ClientListMessage >& list) override;
		void OnRoomList(std::vector< SampleServer::RoomListMessage >& list) override;
		void OnEnterRoom() override;
		void OnLeaveRoom() override;
		void OnRoomAddClient() override;
		void OnRoomDeleteClient() override;
		void OnRoomInformation(const SampleServer::RoomPtr& room) override;
	};
}