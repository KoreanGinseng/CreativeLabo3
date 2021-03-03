#include	"Socket.h"

using namespace Sample;

namespace WinSock {
	bool bInitialize = false;
	int SocketCount_ = 0;

	bool Initialize() {
		//�������ς�
		if (bInitialize)
		{
			return true;
		}
		//WinSock�f�[�^
		WSADATA wdata;
		//WS_32.dll�ǂݍ���
		int re = WSAStartup(MAKEWORD(2, 2), &wdata);
		//�ǂݍ��ݎ��s�H
		if (re)
		{
			return false;
		}
		//�o�[�W�����m�F
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
 * @brief		�R���X�g���N�^
 */
UdpSocket::UdpSocket() 
: Socket_(INVALID_SOCKET) {
}

/**
 * @brief		�f�X�g���N�^
 */
UdpSocket::~UdpSocket() {
	Close();
}
/**
 * @brief		����
 */
bool UdpSocket::Create(void) {
	//WinSock������
	WinSock::Initialize();
	//�\�P�b�g��V�K�쐬
	Close();
	Socket_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (Socket_ < 0)
	{
		Socket_ = INVALID_SOCKET;
		return false;
	}
	u_long val = 1;
	ioctlsocket(Socket_, FIONBIO, &val);
	INFO_LOG("�\�P�b�g�𐶐�");
	//WinSock����
	WinSock::Create();
	return true;
}

/**
 * @brief			�\�P�b�g�̃o�C���h
 * @param[in]		port			�|�[�g�ԍ�
 */
bool UdpSocket::Bind(const uint16_t port) {
	if (Socket_ == INVALID_SOCKET)
	{
		if (!Create())
		{
			return false;
		}
	}
	//�A�h���X�\����
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
	//�l�b�g���[�N�̃f�[�^��ݒ�
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(port);
	tmp_addr.sin_addr.s_addr = INADDR_ANY;
	int re = bind(Socket_, (struct sockaddr*) & tmp_addr, sizeof(struct sockaddr_in));
	if (re < 0)
	{
		return false;
	}
	INFO_LOG("�|�[�g[" << port << "]��Bind");
	return true;
}

/**
 * @brief			�\�P�b�g�̃o�C���h
 * @param[in]		addr			���M�A�h���X
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
 * @brief			�f�[�^�̑��M
 * @param[in]		addr			���M�A�h���X
 * @param[in]		pBuf			���M�o�b�t�@
 * @param[in]		size			���M�o�b�t�@�T�C�Y
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
 * @brief			�f�[�^�̑��M
 * @param[in]		addr			���M�A�h���X
 * @param[in]		port			���M�|�[�g
 * @param[in]		pBuf			���M�o�b�t�@
 * @param[in]		size			���M�o�b�t�@�T�C�Y
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
 * @brief			�f�[�^�̎�M
 * @param[in]		pBuf			��M�o�b�t�@
 * @param[in]		size			��M�o�b�t�@�T�C�Y
 * @param[out]		addr			��M�A�h���X
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
 * @brief			�f�[�^�̎�M
 * @param[in]		pBuf			��M�o�b�t�@
 * @param[in]		size			��M�o�b�t�@�T�C�Y
 * @param[in]		bKeep			��M�o�b�t�@�ۑ��t���O
 * @param[out]		addr			��M�A�h���X
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
 * @brief		�\�P�b�g�����
 */
bool UdpSocket::Close(void) {
	if (Socket_ == INVALID_SOCKET)
	{
		return true;
	}
	closesocket(Socket_);
	Socket_ = INVALID_SOCKET;
	INFO_LOG("�\�P�b�g��ؒf");
	//WinSock�I��
	WinSock::Release();
	return true;
}

/**
 * @brief		��M�f�[�^�����邩���f
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