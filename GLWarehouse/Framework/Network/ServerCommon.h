#pragma once

#include	"RUDPMessage.h"

namespace Sample {

	namespace SampleServer {
		/** 名前の最大文字数 */
		constexpr int ServerNameMax = 32;

		/** 通信バッファサイズ */
		constexpr uint32_t BufferMax = 8192;

		/** RUDP再送時間 */
		constexpr double RUDPRetryTime = 0.15;

		/** 接続維持送信周期 */
		constexpr uint32_t KeepAliveTime = 10;

		/** サーバー内での削除判定周期 */
		constexpr uint32_t ServerDeleteTime = 60;

		/** サーバー処理メッセージ */
		constexpr uint32_t ServerMessage = 0xFA000000;

		/** エラーコード */
		enum class ErrorCode : unsigned int {
			/** サーバーが受け取るべきメッセージじゃないものが送信された */
			NotServerMessage,
			/** 不明なクライアント */
			InvalidClient,
			/** ゲームＩＤが違う */
			IncorrectGameID,
			/** バージョンＩＤが違う */
			IncorrectVersionID,
			/** ルームがない */
			InvalidRoom,
			/** 既にルーム内にいる */
			AlreadyInTheRoom,
			/** ルームに入っていない */
			NotInTheRoom,
			/** ルームに最大人数入室済み */
			MaxClientInTheRoom,
		};

		/** 送信対象 */
		enum class SendTarget : unsigned char {
			/** サーバーへのメッセージ */
			Server,
			/** 全てのクライアントへのメッセージ */
			AllClient,
			/** ルーム内のクライアントへのメッセージ */
			RoomClient,
			/** 特定クライアントへのメッセージ */
			ClientID,
		};

		/** メッセージ種類 */
		enum class MessageType {
			/** クライアント接続メッセージ */
			Connect,
			/** クライアント切断メッセージ */
			DisConnect,
			/** クライアントリスト要求メッセージ */
			ClientList,
			/** ルーム作成要求メッセージ */
			CreateRoom,
			/** ランダムな部屋への入室＆ルーム作成要求メッセージ */
			CreateAndEnterRandomRoom,
			/** ルームリスト要求メッセージ */
			RoomList,
			/** ルーム入室要求メッセージ */
			EnterRoom,
			/** ルーム退出要求メッセージ */
			LeaveRoom,
			/** ルーム情報要求メッセージ */
			RoomInformation,
			/** ルームのクライアントが増えたメッセージ */
			RoomAddClient,
			/** ルームのクライアントが減ったメッセージ */
			RoomDeleteClient,
			/** エラーメッセージ */
			Error,
			/** 接続生存メッセージ */
			KeepAlive,
			/** その他のゲーム内メッセージ */
			Other,
		};

		/** メッセージ種類 */
		enum class SendType : unsigned char {
			/** UDP */
			Udp,
			/** RUDP制御 */
			RUdp,
		};

		/**
		 * サーバーコンフィグ
		 */
		struct ServerConfig {
			/** 接続ＩＰ */
			std::string IP;
			/** 接続ポート */
			uint16_t port;
			/** ゲームＩＤ */
			uint32_t	gameID;
			/** バージョンＩＤ */
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
		 * 接続中クライアント
		 */
		struct ClientInformation {
			/** ユニークＩＤ */
			uint32_t	uid;
			/** 名前 */
			char		name[ServerNameMax];
			/** 最終通信時間 */
			double		lastTime;

			ClientInformation()
			: uid(0)
			, name()
			, lastTime(0)
			{
			}
		};

		/**
		 * ルーム情報
		 */
		struct RoomData {
		};

		/**
		 * ルーム情報
		 */
		struct Room {
			/** ユニークＩＤ */
			uint32_t							uid;
			/** 名前 */
			char								name[ServerNameMax];
			/** 最大人数 */
			uint32_t							maxClient;
			/** 接続中クライアントID */
			std::vector< ClientInformation >	clients;
			/** ルーム登録情報 */
			std::vector< RoomData >				roomData;
		};
		using RoomPtr = std::shared_ptr< Room >;

#pragma pack(push, 1)
		/**
		 * メッセージヘッダー
		 */
		struct ServerHeader {
			/** 送信者のユニークＩＤ */
			uint32_t	senduid;
			/**
			 * 送信先のユニークＩＤ
			 * targetがClientIDの場合のみ有効
			 */
			uint32_t	recvuid;
			/** 送信ターゲット */
			SendTarget	target;
			/** メッセージタイプ */
			MessageType	type;
			/** 送信タイプ */
			SendType	sendType;
		};

		/**
		 * 接続メッセージ
		 */
		struct ConnectMessage {
			/** 接続者の名前 */
			char		name[ServerNameMax];
			/** ゲームＩＤ */
			uint32_t	gameID;
			/** バージョンＩＤ */
			uint32_t	versionID;
		};

		/**
		 * 接続中クライアント
		 */
		struct ClientListMessage {
			/** ユニークＩＤ */
			uint32_t	uid;
			/** 名前 */
			char		name[ServerNameMax];
		};

		/**
		 * ルーム作成
		 */
		struct RoomCreateMessage {
			/** 名前 */
			char		name[ServerNameMax];
			/** 最大人数 */
			uint32_t	maxClient;
		};

		/**
		 * ルーム
		 */
		struct RoomListMessage {
			/** ユニークＩＤ */
			uint32_t	uid;
			/** 名前 */
			char		name[ServerNameMax];
		};

		/**
		 * エラーメッセージ
		 */
		struct ErrorMessage {
			/** エラーコード */
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