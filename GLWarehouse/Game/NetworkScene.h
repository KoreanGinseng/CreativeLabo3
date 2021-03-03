#pragma once

#include	"Stage.h"
#include	"NetworkKeyMove.h"

namespace Sample {

	/**
	 * @brief		ゲームメインシーンの定義
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
		 * @brief		コンストラクタ
		 */
		NetworkScene();

		/**
		 * @brief		デストラクタ
		 */
		virtual ~NetworkScene();

		/**
		 * @brief		継承先のシーンの構築をおこなう
		 */
		void OnCreate() override;

		/**
		 * @brief		継承先のシーンの更新処理をおこなう
		 */
		void OnUpdate() override;

		/**
		 * @brief		継承先のシーンの更新後処理をおこなう
		 */
		void OnLateUpdate() override;

		/** ネットワーク系のイベント定義 */
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