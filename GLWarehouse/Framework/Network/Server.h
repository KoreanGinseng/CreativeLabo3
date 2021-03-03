#pragma once

#include	"ServerCommon.h"

namespace Sample {

	namespace SampleServer {
		/**
		 * @brief			サーバー基底インターフェイス
		 */
		class IServer {
		public:
			/**
			 * 接続中クライアント
			 */
			struct ConnectClient : public ClientInformation {
				/** アドレス */
				SocketAddr	addr;
				/** 所属ルーム */
				RoomPtr		Room;
			};
			using ConnectClientPtr = std::shared_ptr< ConnectClient >;

			/**
			 * @brief		生成
			 */
			virtual bool Create(const ServerConfig& config) = 0;

			/**
			 * @brief		全員に送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		type			送信メッセージ識別子
			 * @param[in]		ReTime			再送までの時間
			 */
			virtual void SendAll(const void* pData, uint32_t Size, uint32_t type, double ReTime = 0) = 0;

			/**
			 * @brief		送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		tuid			送信対象ＵＩＤ
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			virtual bool Send(const void* pData, uint32_t Size, uint32_t tuid, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		it				送信対象
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			virtual bool Send(const void* pData, uint32_t Size, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		送信
			 * @param[in]		room			送信ルーム
			 * @param[in]		it				送信対象
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			virtual bool SendRoom(const RoomPtr& room, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		送信
			 * @param[in]		code			エラーコード
			 * @param[in]		tuid			送信対象ＵＩＤ
			 * @param[in]		sendtype		送信タイプ
			 */
			virtual bool SendError(ErrorCode code, uint32_t tuid, SendType sendtype) = 0;

			/**
			 * @brief		送信
			 * @param[in]		code			エラーコード
			 * @param[in]		addr			送信対象アドレス
			 * @param[in]		sendtype		送信タイプ
			 */
			virtual bool SendError(ErrorCode code, SocketAddr addr, SendType sendtype) = 0;

			/**
			 * @brief		動作
			 */
			virtual void Run() = 0;

			/**
			 * @brief		クライアントの追加
			 */
			virtual void AddClient(ConnectClientPtr ct) = 0;

			/**
			 * @brief		クライアントの削除
			 */
			virtual void DeleteClientImpl(ConnectClientPtr ct) = 0;

			/**
			 * @brief		クライアントの削除
			 */
			virtual void DeleteClient(void) = 0;

			/**
			 * @brief		クライアントの削除
			 */
			virtual void DeleteClient(uint32_t id) = 0;

			/**
			 * @brief		ルームの追加
			 */
			virtual void AddRoom(RoomPtr ct) = 0;

			/**
			 * @brief		ルームの削除
			 */
			virtual void DeleteRoom(void) = 0;

			/**
			 * @brief		ルームの削除
			 */
			virtual void DeleteRoom(uint32_t id) = 0;

			/**
			 * @brief		クライアントのルーム入室
			 */
			virtual void EnterRoom(RoomPtr room, ConnectClientPtr ct) = 0;

			/**
			 * @brief		クライアントのルーム退出
			 */
			virtual void LeaveRoom(ConnectClientPtr ct) = 0;

			/**
			 * @brief		解放
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
			 * @brief		受信メッセージのパース
			 */
			virtual void ParseMessage(char* pData, RUDPMessage::Header& header, SocketAddr& addr) = 0;
			/**
			 * @brief		接続メッセージ
			 */
			virtual void ParseConnectMessage(ConnectClientPtr client, ConnectMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) = 0;
			/**
			 * @brief		クライアントリスト要求メッセージ
			 */
			virtual void ParseClientListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) = 0;
			/**
			 * @brief		ルーム作成メッセージ
			 */
			virtual void ParseRoomCreateMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) = 0;
			/**
			 * @brief		ルーム作成メッセージ
			 */
			virtual void ParseRandomRoomMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) = 0;
			/**
			 * @brief		ルームリスト要求メッセージ
			 */
			virtual void ParseRoomListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) = 0;
			/**
			 * @brief		その他のメッセージ
			 */
			virtual void ParseOtherMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) = 0;
		};
		//ポインタ置き換え
		using ServerPtr = std::shared_ptr<IServer>;

		/**
		 * @brief			サーバー
		 */
		class Server : public IServer {
		public:
		protected:
			/** サーバー設定 */
			ServerConfig					config_;
			/** UDPソケット */
			UdpSocketPtr					socket_;
			/** RUDP */
			RUDPMessage						RUDP_;
			/** 接続中クライアント */
			std::list< ConnectClientPtr >	clients_;
			/** ルーム */
			std::list< RoomPtr >			rooms_;
			/** クライアントロック */
			std::mutex						clientlock_;
			/** ルームロック */
			std::mutex						roomlock_;
			/** サーバースレッド */
			std::thread						thread_;
			/** 動作フラグ */
			bool							bEnabled;
			/** 乱数生成 */
			std::mt19937					mtRandom;
		public:
			/**
			 * @brief		コンストラクタ
			 */
			Server();

			/**
			 * @brief		デストラクタ
			 */
			virtual ~Server();

			/**
			 * @brief		生成
			 */
			bool Create(const ServerConfig& config) override;

			/**
			 * @brief		全員に送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		type			送信メッセージ識別子
			 * @param[in]		ReTime			再送までの時間
			 */
			void SendAll(const void* pData, uint32_t Size, uint32_t type, double ReTime = 0) override;

			/**
			 * @brief		送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		tuid			送信対象ＵＩＤ
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			bool Send(const void* pData, uint32_t Size, uint32_t tuid, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		it				送信対象
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			bool Send(const void* pData, uint32_t Size, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		送信
			 * @param[in]		room			送信ルーム
			 * @param[in]		it				送信対象
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			bool SendRoom(const RoomPtr& room, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		送信
			 * @param[in]		code			エラーコード
			 * @param[in]		tuid			送信対象ＵＩＤ
			 * @param[in]		sendtype		送信タイプ
			 */
			bool SendError(ErrorCode code, uint32_t tuid, SendType sendtype) override;

			/**
			 * @brief		送信
			 * @param[in]		code			エラーコード
			 * @param[in]		addr			送信対象アドレス
			 * @param[in]		sendtype		送信タイプ
			 */
			bool SendError(ErrorCode code, SocketAddr addr, SendType sendtype) override;

			/**
			 * @brief		動作
			 */
			void Run() override;

			/**
			 * @brief		クライアントの追加
			 */
			void AddClient(ConnectClientPtr ct) override;

			/**
			 * @brief		クライアントの削除
			 */
			void DeleteClientImpl(ConnectClientPtr ct) override;

			/**
			 * @brief		クライアントの削除
			 */
			void DeleteClient(void) override;

			/**
			 * @brief		クライアントの削除
			 */
			void DeleteClient(uint32_t id) override;

			/**
			 * @brief		ルームの追加
			 */
			void AddRoom(RoomPtr ct) override;

			/**
			 * @brief		ルームの削除
			 */
			void DeleteRoom(void) override;

			/**
			 * @brief		ルームの削除
			 */
			void DeleteRoom(uint32_t id) override;

			/**
			 * @brief		クライアントのルーム入室
			 */
			void EnterRoom(RoomPtr room, ConnectClientPtr ct) override;

			/**
			 * @brief		クライアントのルーム退出
			 */
			void LeaveRoom(ConnectClientPtr ct) override;

			/**
			 * @brief		解放
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
			 * @brief		受信メッセージのパース
			 */
			void ParseMessage(char* pData, RUDPMessage::Header& header, SocketAddr& addr) override;
			/**
			 * @brief		接続メッセージ
			 */
			void ParseConnectMessage(ConnectClientPtr client, ConnectMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) override;
			/**
			 * @brief		クライアントリスト要求メッセージ
			 */
			void ParseClientListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) override;
			/**
			 * @brief		ルーム作成メッセージ
			 */
			void ParseRoomCreateMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) override;
			/**
			 * @brief		ルーム作成メッセージ
			 */
			void ParseRandomRoomMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) override;
			/**
			 * @brief		ルームリスト要求メッセージ
			 */
			void ParseRoomListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) override;
			/**
			 * @brief		その他のメッセージ
			 */
			void ParseOtherMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) override;
		};
	}
}
