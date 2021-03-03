#include	"Socket.h"

using namespace Sample;

namespace WinSock {
	bool bInitialize = false;
	int SocketCount_ = 0;

	bool Initialize() {
		//初期化済み
		if (bInitialize)
		{
			return true;
		}
		//WinSockデータ
		WSADATA wdata;
		//WS_32.dll読み込み
		int re = WSAStartup(MAKEWORD(2, 2), &wdata);
		//読み込み失敗？
		if (re)
		{
			return false;
		}
		//バージョン確認
		if (LOBYTE(wdata.wVersion) != 2 || HIBYTE(wdata.wVersion) != 2)
		{
			WSACleanup();
			return false;
		}
		bInitialize = true;
		INFO_LOG("WSAStartup...");
		return true;
	}

	bool Create() {
		SocketCount_++;
		return true;
	}

	bool Release() {
		SocketCount_--;
		if (SocketCount_ > 0)
		{
			return false;
		}
		WSACleanup();
		bInitialize = false;
		INFO_LOG("WSACleanup...");
		return true;
	}
}
/**
 * @brief		コンストラクタ
 */
UdpSocket::UdpSocket() 
: Socket_(INVALID_SOCKET) {
}

/**
 * @brief		デストラクタ
 */
UdpSocket::~UdpSocket() {
	Close();
}
/**
 * @brief		生成
 */
bool UdpSocket::Create(void) {
	//WinSock初期化
	WinSock::Initialize();
	//ソケットを新規作成
	Close();
	Socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (Socket_ < 0)
	{
		Socket_ = INVALID_SOCKET;
		return false;
	}
	u_long val = 1;
	ioctlsocket(Socket_, FIONBIO, &val);
	INFO_LOG("ソケットを生成");
	//WinSock生成
	WinSock::Create();
	return true;
}

/**
 * @brief			ソケットのバインド
 * @param[in]		port			ポート番号
 */
bool UdpSocket::Bind(const uint16_t port) {
	if (Socket_ == INVALID_SOCKET)
	{
		if (!Create())
		{
			return false;
		}
	}
	//アドレス構造体
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
	//ネットワークのデータを設定
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(port);
	tmp_addr.sin_addr.s_addr = INADDR_ANY;
	int re = bind(Socket_, (struct sockaddr*) & tmp_addr, sizeof(struct sockaddr_in));
	if (re < 0)
	{
		return false;
	}
	INFO_LOG("ポート[" << port << "]とBind");
	return true;
}

/**
 * @brief			ソケットのバインド
 * @param[in]		addr			送信アドレス
 */
bool UdpSocket::Bind(const SocketAddr& addr) {
	if (Socket_ == INVALID_SOCKET)
	{
		if (!Create())
		{
			return false;
		}
	}
	int re = bind(Socket_, (struct sockaddr*) & addr, sizeof(struct sockaddr_in));
	if (re < 0)
	{
		return false;
	}
	return true;
}

/**
 * @brief			データの送信
 * @param[in]		addr			送信アドレス
 * @param[in]		pBuf			送信バッファ
 * @param[in]		size			送信バッファサイズ
 */
uint32_t UdpSocket::Send(const SocketAddr& addr, const void* pBuf, const uint32_t size) {
	if (Socket_ == INVALID_SOCKET || !pBuf)
	{
		return false;
	}
	int ret = sendto(Socket_, (char*)pBuf, size, 0, reinterpret_cast<const sockaddr*>(&addr), sizeof(SocketAddr));
	if (ret < 0)
	{
		return false;
	}
	return ret;
}

/**
 * @brief			データの送信
 * @param[in]		addr			送信アドレス
 * @param[in]		port			送信ポート
 * @param[in]		pBuf			送信バッファ
 * @param[in]		size			送信バッファサイズ
 */
uint32_t UdpSocket::Send(uint32_t addr, uint16_t port, const void* pBuf, const uint32_t size) {
	if (Socket_ == INVALID_SOCKET || !pBuf)
	{
		return false;
	}
	SocketAddr sockAddrIn;
	memset(&sockAddrIn, 0, sizeof(SocketAddr));
	sockAddrIn.sin_addr.s_addr = addr;
	sockAddrIn.sin_family = AF_INET;
	sockAddrIn.sin_port = htons(port);
	int ret = sendto(Socket_, (char*)pBuf, size, 0, reinterpret_cast<const sockaddr*>(&sockAddrIn), sizeof(SocketAddr));
	if (ret < 0)
	{
		return false;
	}
	return ret;
}

/**
 * @brief			データの受信
 * @param[in]		pBuf			受信バッファ
 * @param[in]		size			受信バッファサイズ
 * @param[out]		addr			受信アドレス
 */
uint32_t UdpSocket::Receive(void* pBuf, const uint32_t size, SocketAddr& addr) {
	if (Socket_ == INVALID_SOCKET || !pBuf)
	{
		return false;
	}
	int sockAddrLen = sizeof(SocketAddr);
	int ret = recvfrom(Socket_, (char*)pBuf, size, 0, reinterpret_cast<sockaddr*>(&addr), &sockAddrLen);
	if (ret <= 0)
	{
		return false;
	}
	return ret;
}
/**
 * @brief			データの受信
 * @param[in]		pBuf			受信バッファ
 * @param[in]		size			受信バッファサイズ
 * @param[in]		bKeep			受信バッファ保存フラグ
 * @param[out]		addr			受信アドレス
 */
uint32_t UdpSocket::Receive(void* pBuf, const uint32_t size, bool bKeep, SocketAddr& addr) {
	if (Socket_ == INVALID_SOCKET || !pBuf)
	{
		return false;
	}
	int sockAddrLen = sizeof(SocketAddr);
	int ret = recvfrom(Socket_, (char*)pBuf, size, ((bKeep) ? MSG_PEEK : 0), reinterpret_cast<sockaddr*>(&addr), &sockAddrLen);
	if (ret <= 0)
	{
		return false;
	}
	return ret;
}

/**
 * @brief		ソケットを閉じる
 */
bool UdpSocket::Close(void) {
	if (Socket_ == INVALID_SOCKET)
	{
		return true;
	}
	closesocket(Socket_);
	Socket_ = INVALID_SOCKET;
	INFO_LOG("ソケットを切断");
	//WinSock終了
	WinSock::Release();
	return true;
}

/**
 * @brief		受信データがあるか判断
 */
bool UdpSocket::IsReceive(void) {
	if (Socket_ == INVALID_SOCKET)
	{
		return false;
	}
	static char dummy[2048];
	struct sockaddr sockAddr;
	int sockAddrLen = sizeof(sockAddr);
	int ret = recvfrom(Socket_, dummy, sizeof(dummy), MSG_PEEK, &sockAddr, &sockAddrLen);
	if (ret < 0 && ret != WSAEWOULDBLOCK)
	{
		return false;
	}
	return true;
}