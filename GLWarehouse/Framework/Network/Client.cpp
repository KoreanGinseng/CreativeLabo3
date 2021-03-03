#include	"Client.h"

using namespace Sample;
using namespace Sample::SampleServer;

/**
 * @brief		�R���X�g���N�^
 */
Client::Client()
: info_()
, room_()
, socket_()
, serverAddr_()
, RUDP_()
, OnFunc()
{
}

/**
 * @brief		�f�X�g���N�^
 */
Client::~Client()
{
	Release();
}

/**
 * @brief		����
 */
bool Client::Create(const ServerConfig& sconfig, const char* name, uint16_t port, ClientEventFunctions func)
{
	//�\�P�b�g����
	socket_ = std::make_shared<UdpSocket>();
	if (!socket_->Create())
	{
		ERROR_LOG("�N���C�A���g�������A�|�[�g�������s...");
		return false;
	}

	//�\�P�b�g�o�C���h
	if (!socket_->Bind(port))
	{
		ERROR_LOG("�N���C�A���g�������A�\�P�b�g�o�C���h���s...");
		return false;
	}

	//�l�b�g���[�N�̃f�[�^��ݒ�
	memset(&serverAddr_, 0, sizeof(SocketAddr));
	serverAddr_.sin_family = AF_INET;
	serverAddr_.sin_port = htons(sconfig.port);
	serverAddr_.sin_addr.s_addr = inet_addr(sconfig.IP.c_str());

	//��M�o�^
	OnFunc = func;

	//�ڑ����b�Z�[�W���M
	ConnectMessage cmsg;
	memset(&cmsg, 0, sizeof(cmsg));
	strcpy(cmsg.name, name);
	cmsg.gameID = sconfig.gameID;
	cmsg.versionID = sconfig.versionID;
	Send(&cmsg, sizeof(ConnectMessage), ServerMessage, SendTarget::Server, 0, MessageType::Connect, SendType::RUdp);
	strcpy(info_.name, name);
	ERROR_LOG("�T�[�o�[�ɐڑ�...");
	return true;
}

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
bool Client::Send(const void* pData, uint32_t Size, uint32_t type, SendTarget target, uint32_t tuid, MessageType msgtype, SendType sendtype) {
	if (socket_ == nullptr)
	{
		return false;
	}
	uint32_t msize = Size + sizeof(ServerHeader);
	uint8_t* pBuf = new uint8_t[msize];
	ServerHeader* pHeader = (ServerHeader*)pBuf;
	pHeader->senduid = info_.uid;
	pHeader->recvuid = tuid;
	pHeader->target = target;
	pHeader->type = msgtype;
	pHeader->sendType = sendtype;
	double ReTime = 0;
	if (sendtype == SendType::RUdp)
	{
		ReTime = RUDPRetryTime;
	}
	if (Size > 0)
	{
		memcpy(&pBuf[sizeof(ServerHeader)], pData, Size);
	}
	RUDP_.Send(socket_, serverAddr_, pBuf, msize, type, ReTime);
	SAFE_DELETE_ARRAY(pBuf);
	return true;
}


/**
 * @brief		�N���C�A���g���X�g�v��
 */
void Client::RequestClientList() {
	Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::ClientList, SendType::RUdp);
}


/**
 * @brief		���[�����X�g�v��
 */
void Client::RequestRoomList() {
	Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::RoomList, SendType::RUdp);
}


/**
 * @brief		���[���쐬�v��
 * @param[in]		name			���[����
 * @param[in]		maxclient		�ő�ڑ��l��
 */
void Client::CreateRoom(const char* name, uint32_t maxclient) {
	RoomCreateMessage cmsg;
	memset(&cmsg, 0, sizeof(cmsg));
	strcpy(cmsg.name, name);
	cmsg.maxClient = maxclient;
	Send(&cmsg, sizeof(RoomCreateMessage), ServerMessage, SendTarget::Server, 0, MessageType::CreateRoom, SendType::RUdp);
}

/**
 * @brief		���[���쐬�v��
 */
void Client::RandomRoom(const char* name, uint32_t maxclient) {
	RoomCreateMessage cmsg;
	memset(&cmsg, 0, sizeof(cmsg));
	strcpy(cmsg.name, name);
	cmsg.maxClient = maxclient;
	Send(&cmsg, sizeof(RoomCreateMessage), ServerMessage, SendTarget::Server, 0, MessageType::CreateAndEnterRandomRoom, SendType::RUdp);
}

/**
 * @brief		���[�������v��
 * @param[in]		uid				�������郋�[���h�c
 */
void Client::EnterRoom(uint32_t uid) {
	Send(&uid, sizeof(uid), ServerMessage, SendTarget::Server, 0, MessageType::EnterRoom, SendType::RUdp);
}

/**
 * @brief		���[���ޏo�v��
 */
void Client::LeaveRoom() {
	Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::LeaveRoom, SendType::RUdp);
}


/**
 * @brief		����
 */
void Client::Run() {
	//����������Ă��Ȃ�
	if (socket_ == nullptr)
	{
		return;
	}
	//��M�p�o�b�t�@
	char buf[BufferMax];
	//��M�p�w�b�_�[
	RUDPMessage::Header header;
	//��M�A�h���X
	SocketAddr addr;
	//RUDP�X�V
	RUDP_.Update();
	//��M����
	while (RUDP_.Receive(buf, BufferMax, header, addr, socket_))
	{
		if (serverAddr_.sin_addr.S_un.S_addr != addr.sin_addr.S_un.S_addr)
		{
			ERROR_LOG("Unknown Receive!!");
			continue;
		}
		ParseMessage(buf, header);
	}
	//�ڑ��ێ����M
	double nt = FrameTimer::Instance().NowTime();
	if (nt - info_.lastTime >= KeepAliveTime)
	{
		info_.lastTime = nt;
		Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::KeepAlive, SendType::Udp);
	}
}

/**
 * @brief		��M���b�Z�[�W�̃p�[�X
 */
void Client::ParseMessage(char* pData, RUDPMessage::Header& header) {
	//���b�Z�[�W����擪�̃w�b�_�[���o��
	ServerHeader* h = (ServerHeader*)pData;
	//���b�Z�[�W���Ƃ̏���
	switch (h->type)
	{
	case MessageType::Connect:
		memcpy(&info_.uid, &pData[sizeof(ServerHeader)], sizeof(info_.uid));
		INFO_LOG("ConnectServer[" << info_.uid << "]");
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnConnect();
		}
		else if (OnFunc.OnConnect != nullptr)
		{
			OnFunc.OnConnect();
		}
		break;

	case MessageType::ClientList:
		ParseClientListMessage(pData, header);
		break;

	case MessageType::CreateRoom:
		room_ = std::make_shared<Room>();
		ParseRoomMessage(pData, header, room_);
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnRoomCreate();
		}
		else if (OnFunc.OnRoomCreate != nullptr)
		{
			OnFunc.OnRoomCreate();
		}
		break;

	case MessageType::EnterRoom:
		room_ = std::make_shared<Room>();
		ParseRoomMessage(pData, header, room_);
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnEnterRoom();
		}
		else if (OnFunc.OnEnterRoom != nullptr)
		{
			OnFunc.OnEnterRoom();
		}
		break;

	case MessageType::LeaveRoom:
		room_.reset();
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnLeaveRoom();
		}
		else if (OnFunc.OnLeaveRoom != nullptr)
		{
			OnFunc.OnLeaveRoom();
		}
		break;

	case MessageType::RoomInformation:
		//�����̃��[���Ȃ�����X�V
		if (room_ != nullptr)
		{
			uint32_t uid;
			memcpy(&uid, &pData[sizeof(ServerHeader)], sizeof(uid));
			if (room_->uid == uid)
			{
				ParseRoomMessage(pData, header, room_);
			}
			else
			{
				//�����ȊO�̃��[���Ƃ��Ĉꎞ�I�ɐ���
				RoomPtr temp = std::make_shared<Room>();
				ParseRoomMessage(pData, header, temp);
				if (OnFunc.eventInterface != nullptr)
				{
					OnFunc.eventInterface->OnRoomInformation(temp);
				}
				else if (OnFunc.OnRoomInformation != nullptr)
				{
					OnFunc.OnRoomInformation(temp);
				}
			}
		}
		else
		{
			//�����ȊO�̃��[���Ƃ��Ĉꎞ�I�ɐ���
			RoomPtr temp = std::make_shared<Room>();
			ParseRoomMessage(pData, header, temp);
			if (OnFunc.eventInterface != nullptr)
			{
				OnFunc.eventInterface->OnRoomInformation(temp);
			}
			else if (OnFunc.OnRoomInformation != nullptr)
			{
				OnFunc.OnRoomInformation(temp);
			}
		}
		break;

	case MessageType::RoomAddClient:
		ParseRoomMessage(pData, header, room_);
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnRoomAddClient();
		}
		else if (OnFunc.OnRoomAddClient != nullptr)
		{
			OnFunc.OnRoomAddClient();
		}
		break;

	case MessageType::RoomDeleteClient:
		ParseRoomMessage(pData, header, room_);
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnRoomDeleteClient();
		}
		else if (OnFunc.OnRoomDeleteClient != nullptr)
		{
			OnFunc.OnRoomDeleteClient();
		}
		break;

	case MessageType::RoomList:
		ParseRoomListMessage(pData, header);
		break;

	case MessageType::Error:
		ERROR_LOG("ServerError!! ErrorCode[" << EnumAsNumber(((ErrorMessage*)(&pData[sizeof(ServerHeader)]))->code) << "]");
		if (((ErrorMessage*)(&pData[sizeof(ServerHeader)]))->code == ErrorCode::InvalidClient)
		{
			//�s���ȃN���C�A���g�Ȃ��߃T�[�o�[����ؒf
			socket_.reset();
			info_.uid = 0;
		}
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnError(((ErrorMessage*)(&pData[sizeof(ServerHeader)]))->code);
		}
		else if (OnFunc.OnError != nullptr)
		{
			OnFunc.OnError(((ErrorMessage*)(&pData[sizeof(ServerHeader)]))->code);
		}
		break;

	case MessageType::Other:
		if (OnFunc.eventInterface != nullptr)
		{
			OnFunc.eventInterface->OnReceive(*h, &pData[sizeof(ServerHeader)], header);
		}
		else if (OnFunc.OnReceive != nullptr)
		{
			OnFunc.OnReceive(*h, &pData[sizeof(ServerHeader)], header);
		}
		break;
	}
}

/**
 * @brief		��M���b�Z�[�W�̃p�[�X
 */
void Client::ParseClientListMessage(char* pData, RUDPMessage::Header& header) {
	std::vector<ClientListMessage > clist;
	uint32_t cnt = (header.Size - sizeof(ServerHeader)) / sizeof(ClientListMessage);
	clist.resize(cnt);
	ClientListMessage* pclist = (ClientListMessage*)(&pData[sizeof(ServerHeader)]);
	for (uint32_t i = 0; i < cnt; i++)
	{
		clist[i].uid = pclist->uid;
		strcpy(clist[i].name, pclist->name);
		pclist++;
	}
	if (OnFunc.eventInterface != nullptr)
	{
		OnFunc.eventInterface->OnClientList(clist);
	}
	else if (OnFunc.OnClientList != nullptr)
	{
		OnFunc.OnClientList(clist);
	}
}

/**
 * @brief		��M���b�Z�[�W�̃p�[�X
 */
void Client::ParseRoomMessage(char* pData, RUDPMessage::Header& header, RoomPtr room) {
	//���[�����Ȃ�
	if (room == nullptr)
	{
		return;
	}
	uint32_t offset = sizeof(ServerHeader);
	//�f�[�^�����o���Ă���
	memcpy(&room->uid, &pData[offset], sizeof(room->uid));
	offset += sizeof(room->uid);
	memcpy(room->name, &pData[offset], sizeof(room->name));
	offset += sizeof(room->name);
	memcpy(&room->maxClient, &pData[offset], sizeof(room->maxClient));
	offset += sizeof(room->maxClient);
	//�����N���C�A���g���
	uint32_t size;
	memcpy(&size, &pData[offset], sizeof(size));
	offset += sizeof(size);
	room->clients.resize(size);
	for (auto& client : room->clients)
	{
		memcpy(&client.uid, &pData[offset], sizeof(client.uid));
		offset += sizeof(client.uid);
		memcpy(&client.name, &pData[offset], sizeof(client.name));
		offset += sizeof(client.name);
	}
	//���[�����
	memcpy(&size, &pData[offset], sizeof(size));
	offset += sizeof(size);
	room->roomData.resize(size);
	for (auto& data : room->roomData)
	{
	}
}

/**
 * @brief		��M���b�Z�[�W�̃p�[�X
 */
void Client::ParseRoomListMessage(char* pData, RUDPMessage::Header& header) {
	std::vector<RoomListMessage > clist;
	uint32_t cnt = (header.Size - sizeof(ServerHeader)) / sizeof(RoomListMessage);
	clist.resize(cnt);
	RoomListMessage* pclist = (RoomListMessage*)(&pData[sizeof(ServerHeader)]);
	for (uint32_t i = 0; i < cnt; i++)
	{
		clist[i].uid = pclist->uid;
		strcpy(clist[i].name, pclist->name);
		pclist++;
	}
	if (OnFunc.eventInterface != nullptr)
	{
		OnFunc.eventInterface->OnRoomList(clist);
	}
	else if (OnFunc.OnRoomList != nullptr)
	{
		OnFunc.OnRoomList(clist);
	}
}

/**
 * @brief		���
 */
void Client::Release() {
	//�ؒf���b�Z�[�W���M
	if (socket_ != nullptr)
	{
		Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::DisConnect, SendType::Udp);
	}
	socket_.reset();
	info_.uid = 0;
}