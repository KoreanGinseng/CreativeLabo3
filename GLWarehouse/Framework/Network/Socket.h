#pragma once

#include	"../Common.h"

namespace Sample {

	/**
	 * @brief			ソケット関連置き換え
	 */
	using SocketDescriptor = SOCKET;
	using SocketAddr = sockaddr_in;

	/**
	 * @brief			ソケット基底インターフェイス
	 */
	class ISocket {
	public:
		/**
		 * @brief		生成
		 */
		virtual bool Create(void) = 0;

		/**
		 * @brief		ソケットを閉じる
		 */
		virtual bool Close(void) = 0;

		virtual SocketDescriptor Socket(void) = 0;
	};

	/**
	 * @brief			UDPソケット基底インターフェイス
	 */
	class IUdpSocket : public ISocket {
	public:
		/**
		 * @brief			ソケットのバインド
		 * @param[in]		port			ポート番号
		 */
		virtual bool Bind(const uint16_t port) = 0;
		/**
		 * @brief			ソケットのバインド
		 * @param[in]		addr			送信アドレス
		 */
		virtual bool Bind(const SocketAddr& addr) = 0;

		/**
		 * @brief			データの送信
		 * @param[in]		addr			送信アドレス
		 * @param[in]		pBuf			送信バッファ
		 * @param[in]		size			送信バッファサイズ
		 */
		virtual uint32_t Send(const SocketAddr& addr, const void* pBuf, const uint32_t size) = 0;
		/**
		 * @brief			データの送信
		 * @param[in]		addr			送信アドレス
		 * @param[in]		port			送信ポート
		 * @param[in]		pBuf			送信バッファ
		 * @param[in]		size			送信バッファサイズ
		 */
		virtual uint32_t Send(uint32_t addr, uint16_t port, const void* pBuf, const uint32_t size) = 0;

		/**
		 * @brief			データの受信
		 * @param[in]		pBuf			受信バッファ
		 * @param[in]		size			受信バッファサイズ
		 * @param[out]		addr			受信アドレス
		 */
		virtual uint32_t Receive(void* pBuf, const uint32_t size, SocketAddr& addr) = 0;
		/**
		 * @brief			データの受信
		 * @param[in]		pBuf			受信バッファ
		 * @param[in]		size			受信バッファサイズ
		 * @param[in]		bKeep			受信バッファ保存フラグ
		 * @param[out]		addr			受信アドレス
		 */
		virtual uint32_t Receive(void* pBuf, const uint32_t size, bool bKeep, SocketAddr& addr) = 0;

		/**
		 * @brief		受信データがあるか判断
		 */
		virtual bool IsReceive(void) = 0;
	};
	//ポインタ置き換え
	using UdpSocketPtr = std::shared_ptr<IUdpSocket>;

	/**
	 * @brief			UDPソケット
	 */
	class UdpSocket : public IUdpSocket {
	protected:
		/** ソケット */
		SocketDescriptor	Socket_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		UdpSocket();

		/**
		 * @brief		デストラクタ
		 */
		virtual ~UdpSocket();

		/**
		 * @brief		生成
		 */
		bool Create(void) override;

		/**
		 * @brief			ソケットのバインド
		 * @param[in]		port			ポート番号
		 */
		bool Bind(const uint16_t port) override;
		/**
		 * @brief			ソケットのバインド
		 * @param[in]		addr			送信アドレス
		 */
		bool Bind(const SocketAddr& addr) override;

		/**
		 * @brief			データの送信
		 * @param[in]		addr			送信アドレス
		 * @param[in]		pBuf			送信バッファ
		 * @param[in]		size			送信バッファサイズ
		 */
		uint32_t Send(const SocketAddr& addr, const void* pBuf, const uint32_t size) override;
		/**
		 * @brief			データの送信
		 * @param[in]		addr			送信アドレス
		 * @param[in]		port			送信ポート
		 * @param[in]		pBuf			送信バッファ
		 * @param[in]		size			送信バッファサイズ
		 */
		uint32_t Send(uint32_t addr, uint16_t port, const void* pBuf, const uint32_t size) override;

		/**
		 * @brief			データの受信
		 * @param[in]		pBuf			受信バッファ
		 * @param[in]		size			受信バッファサイズ
		 * @param[out]		addr			受信アドレス
		 */
		uint32_t Receive(void* pBuf, const uint32_t size, SocketAddr& addr) override;
		/**
		 * @brief			データの受信
		 * @param[in]		pBuf			受信バッファ
		 * @param[in]		size			受信バッファサイズ
		 * @param[in]		bKeep			受信バッファ保存フラグ
		 * @param[out]		addr			受信アドレス
		 */
		uint32_t Receive(void* pBuf, const uint32_t size, bool bKeep, SocketAddr& addr) override;

		/**
		 * @brief		ソケットを閉じる
		 */
		bool Close(void) override;

		/**
		 * @brief		受信データがあるか判断
		 */
		bool IsReceive(void) override;

		virtual SocketDescriptor Socket(void) { return Socket_; }
	};
}
