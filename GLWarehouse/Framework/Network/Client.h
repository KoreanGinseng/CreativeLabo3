#pragma once

#include	"ServerCommon.h"

namespace Sample {

	namespace SampleServer {
		/**
		 * @brief			クライアント基底インターフェイス
		 */
		class IClient {
		public:
			/** 受信処理*/
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
				 * @brief		個別イベント登録
				 *				ただしeventInterfaceの登録がある場合そちらが優先される
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
				 * @brief		イベント受信用インターフェイスの登録
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
			 * @brief		生成
			 * @param[in]		serverip		サーバーのＩＰアドレス
			 * @param[in]		serverport		サーバーのポート
			 * @param[in]		name			接続名
			 * @param[in]		port			通信ポート
			 * @param[in]		recv			受信関数
			 */
			virtual bool Create(const ServerConfig& sconfig, const char* name, uint16_t port, ClientEventFunctions func) = 0;

			/**
			 * @brief		送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		type			送信メッセージ識別子
			 * @param[in]		target			送信対象
			 * @param[in]		tuid			送信対象ＵＩＤ
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			virtual bool Send(const void* pData, uint32_t Size, uint32_t type, SendTarget target, uint32_t tuid, MessageType msgtype, SendType sendtype) = 0;

			/**
			 * @brief		クライアントリスト要求
			 */
			virtual void RequestClientList() = 0;

			/**
			 * @brief		ルームリスト要求
			 */
			virtual void RequestRoomList() = 0;

			/**
			 * @brief		ルーム作成要求
			 * @param[in]		name			ルーム名
			 * @param[in]		maxclient		最大接続人数
			 */
			virtual void CreateRoom(const char* name, uint32_t maxclient) = 0;

			/**
			 * @brief		ルーム作成要求
			 * @param[in]		name			ルーム名
			 * @param[in]		maxclient		最大接続人数
			 */
			virtual void RandomRoom(const char* name, uint32_t maxclient) = 0;

			/**
			 * @brief		ルーム入室要求
			 * @param[in]		uid				入室するルームＩＤ
			 */
			virtual void EnterRoom(uint32_t uid) = 0;

			/**
			 * @brief		ルーム退出要求
			 */
			virtual void LeaveRoom() = 0;

			/**
			 * @brief		動作
			 */
			virtual void Run() = 0;

			/**
			 * @brief		解放
			 */
			virtual void Release() = 0;

			/**
			 * @brief		名前取得
			 */
			virtual const char* GetName() const = 0;

			/**
			 * @brief		UID取得
			 */
			virtual const uint32_t GetUID() const = 0;

			/**
			 * @brief		所属ルームの取得
			 */
			virtual const RoomPtr& GetRoom() const = 0;

			/**
			 * @brief		有効判定
			 */
			virtual bool IsEnable() const = 0;
		protected:
			/**
			 * @brief		受信メッセージのパース
			 */
			virtual void ParseMessage(char* pData, RUDPMessage::Header& header) = 0;
			/**
			 * @brief		受信メッセージのパース
			 */
			virtual void ParseClientListMessage(char* pData, RUDPMessage::Header& header) = 0;
			/**
			 * @brief		受信メッセージのパース
			 */
			virtual void ParseRoomMessage(char* pData, RUDPMessage::Header& header, RoomPtr room) = 0;
			/**
			 * @brief		受信メッセージのパース
			 */
			virtual void ParseRoomListMessage(char* pData, RUDPMessage::Header& header) = 0;
		};
		//ポインタ置き換え
		using ClientPtr = std::shared_ptr<IClient>;

		/**
		 * @brief			サーバー
		 */
		class Client : public IClient {
		public:
		protected:
			/** 情報 */
			ClientInformation			info_;
			/** 所属ルーム */
			RoomPtr						room_;
			/** UDPソケット */
			UdpSocketPtr				socket_;
			/** サーバーアドレス */
			SocketAddr					serverAddr_;
			/** RUDP */
			RUDPMessage					RUDP_;
			/** 受信処理*/
			ClientEventFunctions		OnFunc;
		public:
			/**
			 * @brief		コンストラクタ
			 */
			Client();

			/**
			 * @brief		デストラクタ
			 */
			virtual ~Client();

			/**
			 * @brief		生成
			 * @param[in]		serverip		サーバーのＩＰアドレス
			 * @param[in]		serverport		サーバーのポート
			 * @param[in]		name			接続名
			 * @param[in]		port			通信ポート
			 * @param[in]		recv			受信関数
			 */
			virtual bool Create(const ServerConfig& sconfig, const char* name, uint16_t port, ClientEventFunctions func) override;

			/**
			 * @brief		送信
			 * @param[in]		pData			送信バッファ
			 * @param[in]		Size			送信バッファサイズ
			 * @param[in]		type			送信メッセージ識別子
			 * @param[in]		target			送信対象
			 * @param[in]		tuid			送信対象ＵＩＤ
			 * @param[in]		messagetype		メッセ―ジタイプ
			 * @param[in]		sendtype		送信タイプ
			 */
			bool Send(const void* pData, uint32_t Size, uint32_t type, SendTarget target, uint32_t tuid, MessageType msgtype, SendType sendtype) override;

			/**
			 * @brief		クライアントリスト要求
			 */
			void RequestClientList() override;

			/**
			 * @brief		ルームリスト要求
			 */
			void RequestRoomList() override;

			/**
			 * @brief		ルーム作成要求
			 * @param[in]		name			ルーム名
			 * @param[in]		maxclient		最大接続人数
			 */
			void CreateRoom(const char* name, uint32_t maxclient) override;

			/**
			 * @brief		ルーム作成要求
			 */
			void RandomRoom(const char* name, uint32_t maxclient) override;

			/**
			 * @brief		ルーム入室要求
			 * @param[in]		uid				入室するルームＩＤ
			 */
			void EnterRoom(uint32_t uid) override;

			/**
			 * @brief		ルーム退出要求
			 */
			void LeaveRoom() override;

			/**
			 * @brief		動作
			 */
			void Run() override;

			/**
			 * @brief		解放
			 */
			void Release() override;

			/**
			 * @brief		名前取得
			 */
			const char* GetName() const override { return info_.name; }

			/**
			 * @brief		UID取得
			 */
			const uint32_t GetUID() const override { return info_.uid; }

			/**
			 * @brief		所属ルームの取得
			 */
			const RoomPtr& GetRoom() const override { return room_; }

			/**
			 * @brief		有効判定
			 */
			bool IsEnable() const override { return info_.uid > 0; }
		protected:
			/**
			 * @brief		受信メッセージのパース
			 */
			void ParseMessage(char* pData, RUDPMessage::Header& header) override;
			/**
			 * @brief		受信メッセージのパース
			 */
			void ParseClientListMessage(char* pData, RUDPMessage::Header& header) override;
			/**
			 * @brief		受信メッセージのパース
			 */
			void ParseRoomMessage(char* pData, RUDPMessage::Header& header, RoomPtr room) override;
			/**
			 * @brief		受信メッセージのパース
			 */
			void ParseRoomListMessage(char* pData, RUDPMessage::Header& header) override;
		};
	}
}
