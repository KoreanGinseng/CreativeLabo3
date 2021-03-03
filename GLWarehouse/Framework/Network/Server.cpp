#include	"Server.h"

using namespace Sample;
using namespace Sample::SampleServer;

/**
 * @brief		�R���X�g���N�^
 */
Server::Server()
: config_()
, socket_()
, RUDP_()
, clients_()
, rooms_()
, clientlock_()
, roomlock_()
, thread_()
, bEnabled(false)
, mtRandom()
{
}

/**
 * @brief		�f�X�g���N�^
 */
Server::~Server()
{
	Release();
}

/**
 * @brief		����
 */
bool Server::Create(const ServerConfig& config)
{
	//�\�P�b�g����
	socket_ = std::make_shared<UdpSocket>();
	if (!socket_->Create())
	{
		ERROR_LOG("�T�[�o�[�������A�|�[�g�������s...");
		return false;
	}

	//�\�P�b�g�o�C���h
	if (!socket_->Bind(config.port))
	{
		ERROR_LOG("�T�[�o�[�������A�\�P�b�g�o�C���h���s...");
		return false;
	}

	//��������
	std::random_device rnd;
	mtRandom.seed(rnd());

	//�ݒ�o�^
	config_ = config;

	//�X���b�h����
	bEnabled = true;
	thread_ = std::thread([this] { Run(); });
	ERROR_LOG("�T�[�o�[������...");
	return true;
}


/**
 * @brief		�S���ɑ��M
 * @param[in]		pData			���M�o�b�t�@
 * @param[in]		Size			���M�o�b�t�@�T�C�Y
 * @param[in]		type			���M���b�Z�[�W���ʎq
 * @param[in]		ReTime			�đ��܂ł̎���
 */
void Server::SendAll(const void* pData, uint32_t Size, uint32_t type, double ReTime) {
	std::lock_guard<std::mutex> lock(clientlock_);
	for (auto& it : clients_)
	{
		RUDP_.Send(socket_, it->addr, pData, Size, type, ReTime);
	}
}

/**
 * @brief		���M
 * @param[in]		pData			���M�o�b�t�@
 * @param[in]		Size			���M�o�b�t�@�T�C�Y
 * @param[in]		tuid			���M�Ώۂt�h�c
 * @param[in]		messagetype		���b�Z�\�W�^�C�v
 * @param[in]		sendtype		���M�^�C�v
 */
bool Server::Send(const void* pData, uint32_t Size, uint32_t tuid, MessageType msgtype, SendType sendtype) {
	//�f�[�^��M�A�h���X����N���C�A���g����
	auto it = GetClient(tuid);
	if (it == nullptr)
	{
		ERROR_LOG("SendTo UnknownUser");
		return false;
	}
	return Send(pData, Size, it, msgtype, sendtype);
}

/**
 * @brief		���M
 * @param[in]		pData			���M�o�b�t�@
 * @param[in]		Size			���M�o�b�t�@�T�C�Y
 * @param[in]		it				���M�Ώ�
 * @param[in]		messagetype		���b�Z�\�W�^�C�v
 * @param[in]		sendtype		���M�^�C�v
 */
bool Server::Send(const void* pData, uint32_t Size, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) {
	//���b�Z�[�W�\�z
	uint32_t msize = Size + sizeof(ServerHeader);
	uint8_t* pBuf = new uint8_t[msize];
	ServerHeader* pHeader = (ServerHeader*)pBuf;
	pHeader->senduid = 0;
	pHeader->recvuid = it->uid;
	pHeader->target = SendTarget::ClientID;
	pHeader->type = msgtype;
	pHeader->sendType = sendtype;
	uint32_t ReTime = 0;
	if (sendtype == SendType::RUdp)
	{
		ReTime = RUDPRetryTime;
	}
	if (Size > 0)
	{
		memcpy(&pBuf[sizeof(ServerHeader)], pData, Size);
	}
	RUDP_.Send(socket_, it->addr, pBuf, msize, 0, ReTime);
	SAFE_DELETE_ARRAY(pBuf);
	return true;
}


/**
 * @brief		���M
 * @param[in]		room			���M���[��
 * @param[in]		it				���M�Ώ�
 * @param[in]		messagetype		���b�Z�\�W�^�C�v
 * @param[in]		sendtype		���M�^�C�v
 */
bool Server::SendRoom(const RoomPtr& room, ConnectClientPtr& it, MessageType msgtype, SendType sendtype) {
	//���[�����Ȃ�
	if (room == nullptr)
	{
		SendError(ErrorCode::InvalidRoom, it->addr, sendtype);
		return true;
	}
	//���b�Z�[�W�\�z
	constexpr uint32_t msize = 1024;
	uint8_t* pBuf = new uint8_t[msize];
	uint32_t offset = 0;
	//�f�[�^���l�߂Ă���
	memcpy(&pBuf[offset], &room->uid, sizeof(room->uid));
	offset += sizeof(room->uid);
	memcpy(&pBuf[offset], room->name, sizeof(room->name));
	offset += sizeof(room->name);
	memcpy(&pBuf[offset], &room->maxClient, sizeof(room->maxClient));
	offset += sizeof(room->maxClient);
	//�����N���C�A���g���
	uint32_t size = room->clients.size();
	memcpy(&pBuf[offset], &size, sizeof(size));
	offset += sizeof(size);
	for (auto& client : room->clients)
	{
		memcpy(&pBuf[offset], &client.uid, sizeof(client.uid));
		offset += sizeof(client.uid);
		memcpy(&pBuf[offset], &client.name, sizeof(client.name));
		offset += sizeof(client.name);
	}
	//���[�����
	size = room->roomData.size();
	memcpy(&pBuf[offset], &size, sizeof(size));
	offset += sizeof(size);
	for (auto& data : room->roomData)
	{
	}
	Send(pBuf, offset, it, msgtype, sendtype);
	SAFE_DELETE_ARRAY(pBuf);
	return true;
}

/**
 * @brief		���M
 * @param[in]		code			�G���[�R�[�h
 * @param[in]		tuid			���M�Ώۂt�h�c
 * @param[in]		sendtype		���M�^�C�v
 */
bool Server::SendError(ErrorCode code, uint32_t tuid, SendType sendtype) {
	//�f�[�^��M�A�h���X����N���C�A���g����
	auto it = GetClient(tuid);
	if (it == nullptr)
	{
		ERROR_LOG("SendTo UnknownUser");
		return false;
	}
	return SendError(code, it->addr, sendtype);
}

/**
 * @brief		���M
 * @param[in]		code			�G���[�R�[�h
 * @param[in]		addr			���M�ΏۃA�h���X
 * @param[in]		sendtype		���M�^�C�v
 */
bool Server::SendError(ErrorCode code, SocketAddr addr, SendType sendtype) {
	//���b�Z�[�W�\�z
	uint32_t msize = sizeof(ErrorMessage) + sizeof(ServerHeader);
	uint8_t* pBuf = new uint8_t[msize];
	ServerHeader* pHeader = (ServerHeader*)pBuf;
	ErrorMessage* pEMsg = (ErrorMessage*)(&pBuf[sizeof(ServerHeader)]);
	pEMsg->code = code;
	pHeader->senduid = 0;
	pHeader->recvuid = 0;
	pHeader->target = SendTarget::ClientID;
	pHeader->type = MessageType::Error;
	pHeader->sendType = sendtype;
	uint32_t ReTime = 0;
	if (sendtype == SendType::RUdp)
	{
		ReTime = RUDPRetryTime;
	}
	RUDP_.Send(socket_, addr, pBuf, msize, ServerMessage, ReTime);
	SAFE_DELETE_ARRAY(pBuf);
	return true;
}

/**
 * @brief		����
 */
void Server::Run() {
	//��M�p�o�b�t�@
	char buf[BufferMax];
	//��M�p�w�b�_�[
	RUDPMessage::Header header;
	//��M�A�h���X
	SocketAddr addr;
	//�폜�������
	double delTime = FrameTimer::Instance().NowTime();
	//�X���b�h���[�v
	while (bEnabled)
	{
		//RUDP�X�V
		RUDP_.Update();
		//��M����
		while (RUDP_.Receive(buf, BufferMax, header, addr, socket_))
		{
			ParseMessage(buf, header, addr);
		}
		//�폜�������
		if (FrameTimer::Instance().NowTime() - delTime > ServerDeleteTime)
		{
			//�N���C�A���g�폜����
			DeleteClient();
			//���[���폜����
			DeleteRoom();
			delTime = FrameTimer::Instance().NowTime();
		}
		//1msec�E�F�C�g
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

/**
 * @brief		��M���b�Z�[�W�̃p�[�X
 */
void Server::ParseMessage(char* pData, RUDPMessage::Header& header, SocketAddr& addr) {
	//���b�Z�[�W����擪�̃w�b�_�[���o��
	ServerHeader* h = (ServerHeader*)pData;
	//�f�[�^��M�A�h���X����N���C�A���g����
	auto it = GetClient(addr);
	//�s���ȃ��[�U�[����̃��b�Z�[�W
	if (it == nullptr && h->type != MessageType::Connect)
	{
		ERROR_LOG("Receive UnknownUser Message!!");
		SendError(ErrorCode::InvalidClient, addr, SendType::Udp);
		return;
	}
	//���b�Z�[�W���Ƃ̏���
	switch (h->type)
	{
	case MessageType::Connect:
		ParseConnectMessage(it, (ConnectMessage*)(&pData[sizeof(ServerHeader)]), h, addr);
		break;

	case MessageType::DisConnect:
		SERVER_INFO_LOG_LV1("DisConnect [" << it->name << "]");
		DeleteClient(it->uid);
		break;

	case MessageType::ClientList:
		it->lastTime = FrameTimer::Instance().NowTime();
		ParseClientListMessage(it, pData, h, header);
		break;

	case MessageType::CreateRoom:
		it->lastTime = FrameTimer::Instance().NowTime();
		ParseRoomCreateMessage(it, (RoomCreateMessage*)(&pData[sizeof(ServerHeader)]), h, addr);
		break;

	case MessageType::CreateAndEnterRandomRoom:
		it->lastTime = FrameTimer::Instance().NowTime();
		ParseRandomRoomMessage(it, (RoomCreateMessage*)(&pData[sizeof(ServerHeader)]), h, addr);
		break;

	case MessageType::RoomList:
		it->lastTime = FrameTimer::Instance().NowTime();
		ParseRoomListMessage(it, pData, h, header);
		break;

	case MessageType::EnterRoom:
		it->lastTime = FrameTimer::Instance().NowTime();
		if (it->Room)
		{
			SendError(ErrorCode::AlreadyInTheRoom, it->addr, SendType::RUdp);
		}
		else
		{
			EnterRoom(GetRoom(*((uint32_t*)(&pData[sizeof(ServerHeader)]))), it);
		}
		break;

	case MessageType::LeaveRoom:
		it->lastTime = FrameTimer::Instance().NowTime();
		if (it->Room)
		{
			LeaveRoom(it);
		}
		else
		{
			SendError(ErrorCode::NotInTheRoom, it->addr, SendType::RUdp);
		}
		break;

	case MessageType::RoomInformation:
		it->lastTime = FrameTimer::Instance().NowTime();
		SendRoom(GetRoom(*((uint32_t*)(&pData[sizeof(ServerHeader)]))), it, MessageType::RoomInformation, SendType::RUdp);
		break;

	case MessageType::RoomAddClient:
	case MessageType::RoomDeleteClient:
	case MessageType::Error:
		it->lastTime = FrameTimer::Instance().NowTime();
		SendError(ErrorCode::NotServerMessage, it->addr, SendType::RUdp);
		break;

	case MessageType::KeepAlive:
		it->lastTime = FrameTimer::Instance().NowTime();
		break;

	case MessageType::Other:
		it->lastTime = FrameTimer::Instance().NowTime();
		ParseOtherMessage(it, pData, h, header);
		break;
	}
}

/**
 * @brief		�ڑ����b�Z�[�W
 */
void Server::ParseConnectMessage(ConnectClientPtr client, ConnectMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) {
	//�o�[�W������Q�[�����Ⴄ
	if (cmsg->gameID != config_.gameID)
	{
		ERROR_LOG("NotGameID [" << cmsg->gameID << "/" << config_.gameID << "]");
		SendError(ErrorCode::IncorrectGameID, addr, SendType::Udp);
		return;
	}
	if (cmsg->versionID != config_.versionID)
	{
		ERROR_LOG("NotVersionID [" << cmsg->versionID << "/" << config_.versionID << "]");
		SendError(ErrorCode::IncorrectVersionID, addr, SendType::Udp);
		return;
	}
	//�Q�Ƃ��Ȃ��ꍇ�͐V�K�쐬
	if (client == nullptr)
	{
		client = std::make_shared<ConnectClient>();
		strcpy(client->name, cmsg->name);
		client->uid = mtRandom();
		while (IsClient(client->uid) || client->uid == 0)
		{
			client->uid = mtRandom();
		}
		AddClient(client);
	}
	client->addr = addr;
	client->lastTime = FrameTimer::Instance().NowTime();
	//UID�ԐM
	Send(&client->uid, sizeof(client->uid), client, MessageType::Connect, SendType::RUdp);
	SERVER_INFO_LOG_LV1("Connect [" << client->name << "/" << inet_ntoa(client->addr.sin_addr) << "/" << ntohs(client->addr.sin_port) << "]");
}

/**
 * @brief		�N���C�A���g���X�g�v�����b�Z�[�W
 */
void Server::ParseClientListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) {
	std::lock_guard<std::mutex> lock(clientlock_);
	uint32_t size = clients_.size() * sizeof(ClientListMessage);
	char* clistdata = new char[size];
	ClientListMessage* pclist = (ClientListMessage*)clistdata;
	for (auto& it : clients_)
	{
		pclist->uid = it->uid;
		strcpy(pclist->name, it->name);
		pclist++;
	}
	Send(clistdata, size, client, MessageType::ClientList, SendType::RUdp);
	SAFE_DELETE_ARRAY(clistdata);
}

/**
 * @brief		���[���쐬���b�Z�[�W
 */
void Server::ParseRoomCreateMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) {
	RoomPtr room = std::make_shared<Room>();
	strcpy(room->name, cmsg->name);
	room->maxClient = cmsg->maxClient;
	room->uid = mtRandom();
	while (IsRoom(room->uid) || room->uid == 0)
	{
		room->uid = mtRandom();
	}
	room->clients.push_back(*client);
	AddRoom(room);
	client->Room = room;
	//UID�ԐM
	SendRoom(room, client, MessageType::CreateRoom, SendType::RUdp);
	SERVER_INFO_LOG_LV1("RoomCreate [" << client->name << "/" << room->name << "/" << room->uid << "]");
}

/**
 * @brief		���[���쐬���b�Z�[�W
 */
void Server::ParseRandomRoomMessage(ConnectClientPtr client, RoomCreateMessage* cmsg, ServerHeader* pheader, SocketAddr& addr) {
	//���[���ւ̓��o����
	if (client->Room)
	{
		SendError(ErrorCode::AlreadyInTheRoom, client->addr, SendType::RUdp);
		return;
	}
	else
	{
		std::lock_guard<std::mutex> lock(roomlock_);
		//���[���̒���������\�Ȃ��̂�T��
		for (auto& it : rooms_)
		{
			if (it->clients.size() < it->maxClient)
			{
				EnterRoom(it, client);
				return;
			}
		}
	}
	//�Ȃ���΍쐬
	ParseRoomCreateMessage(client, cmsg, pheader, addr);
}

/**
 * @brief		���[�����X�g�v�����b�Z�[�W
 */
void Server::ParseRoomListMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) {
	std::lock_guard<std::mutex> lock(roomlock_);
	uint32_t size = rooms_.size() * sizeof(RoomListMessage);
	char* roomdata = new char[size];
	RoomListMessage* pclist = (RoomListMessage*)roomdata;
	for (auto& it : rooms_)
	{
		pclist->uid = it->uid;
		strcpy(pclist->name, it->name);
		pclist++;
	}
	Send(roomdata, size, client, MessageType::RoomList, SendType::RUdp);
	SAFE_DELETE_ARRAY(roomdata);
}

/**
 * @brief		���̑��̃��b�Z�[�W
 */
void Server::ParseOtherMessage(ConnectClientPtr client, char* pData, ServerHeader* pheader, RUDPMessage::Header& udpheader) {
	//����N���C�A���g�ւ̑��M
	if (pheader->target == SendTarget::ClientID)
	{
		//�f�[�^��M�A�h���X����N���C�A���g����
		auto it = GetClient(pheader->recvuid);
		//�s���ȃ��[�U�[����̃��b�Z�[�W
		if (it == nullptr)
		{
			ERROR_LOG("Receive UnknownUser Message!!");
			return;
		}
		RUDP_.Send(socket_, it->addr, pData, udpheader.Size, udpheader.Type, pheader->sendType == SendType::Udp ? 0 : RUDPRetryTime);
	}
	//���[�����N���C�A���g�ւ̑��M
	else if (pheader->target == SendTarget::RoomClient)
	{
		if (client->Room == nullptr)
		{
			ERROR_LOG("Receive UnknownRoom Message!!");
			return;
		}
		for (auto& client : client->Room->clients)
		{
			//�f�[�^��M�A�h���X����N���C�A���g����
			auto it = GetClient(client.uid);
			//�s���ȃ��[�U�[����̃��b�Z�[�W
			if (it == nullptr)
			{
				ERROR_LOG("Receive UnknownUser Message!!");
				return;
			}
			RUDP_.Send(socket_, it->addr, pData, udpheader.Size, udpheader.Type, pheader->sendType == SendType::Udp ? 0 : RUDPRetryTime);
		}
	}
	//�S�N���C�A���g�ւ̑��M
	else if (pheader->target == SendTarget::AllClient)
	{
		SendAll(pData, udpheader.Size, udpheader.Type, pheader->sendType == SendType::Udp ? 0 : RUDPRetryTime);
	}
}

/**
 * @brief		�N���C�A���g�̒ǉ�
 */
void Server::AddClient(ConnectClientPtr ct) {
	std::lock_guard<std::mutex> lock(clientlock_);
	clients_.push_back(ct);
}



/**
 * @brief		�N���C�A���g�̍폜
 */
void Server::DeleteClientImpl(ConnectClientPtr ct) {
	//���[�����ɂ���ꍇ���[�����̃��[�U�[�ɍ폜�ʒm
	if (ct->Room)
	{
		//�N���C�A���g�̏����폜
		ct->Room->clients.erase(std::remove_if(ct->Room->clients.begin(), ct->Room->clients.end(),
			[ct](const ClientInformation& it) { return it.uid <= ct->uid; }), ct->Room->clients.end());
		//���[���������[�U�[�ւ̃��[�����̑��M
		for (auto& client : ct->Room->clients)
		{
			const auto& it = std::find_if(clients_.begin(), clients_.end(), [ct](const ConnectClientPtr& client) {return client->uid == ct->uid; });
			if (*it == nullptr)
			{
				continue;
			}
			SendRoom(ct->Room, *it, MessageType::RoomDeleteClient, SendType::RUdp);
		}
		ct->Room.reset();
	}
}

/**
 * @brief		�N���C�A���g�̍폜
 */
void Server::DeleteClient(void) {
	std::lock_guard<std::mutex> lock(clientlock_);
	double nt = FrameTimer::Instance().NowTime();
	SERVER_INFO_LOG_LV3("Client Alive Check");
	clients_.erase(std::remove_if(clients_.begin(), clients_.end(),
		[this, nt](const ConnectClientPtr& it) {
			SERVER_INFO_LOG_LV3("\tClient LastConnected[" << it->lastTime << "/" << nt << "]");
			if ((nt - it->lastTime) >= ServerDeleteTime)
			{
				SERVER_INFO_LOG_LV2("\tDeleteClient : Client [" << it->uid << "] Not Connected!!");
				DeleteClientImpl(it);
				return true;
			}
			return false;
		}), clients_.end());
}

/**
 * @brief		�N���C�A���g�̍폜
 */
void Server::DeleteClient(uint32_t id) {
	std::lock_guard<std::mutex> lock(clientlock_);
	clients_.erase(std::remove_if(clients_.begin(), clients_.end(),
		[this, id](const ConnectClientPtr& it) {
			if (it->uid == id)
			{
				DeleteClientImpl(it);
				return true;
			}
			return false;
		}), clients_.end());
}

/**
 * @brief		���[���̒ǉ�
 */
void Server::AddRoom(RoomPtr ct) {
	std::lock_guard<std::mutex> lock(roomlock_);
	rooms_.push_back(ct);
}


/**
 * @brief		���[���̍폜
 */
void Server::DeleteRoom(void) {
	std::lock_guard<std::mutex> lock(roomlock_);
	SERVER_INFO_LOG_LV3("Room Alive Check");
	rooms_.erase(std::remove_if(rooms_.begin(), rooms_.end(),
		[](const RoomPtr& it) {
			if (it->clients.size() <= 0)
			{
				SERVER_INFO_LOG_LV2("\tDeleteRoom : Room [" << it->uid << "] Member Is 0");
				return true;
			}
			return false;
		}), rooms_.end());
}

/**
 * @brief		���[���̍폜
 */
void Server::DeleteRoom(uint32_t id) {
	std::lock_guard<std::mutex> lock(roomlock_);
	rooms_.erase(std::remove_if(rooms_.begin(), rooms_.end(),
		[this, id](const RoomPtr& it) {
			if (it->uid == id)
			{
				//���[���������[�U�[�S���ɑޏo���M
				for (auto& client : it->clients)
				{
					auto ct = GetClient(client.uid);
					if (ct != nullptr)
					{
						ct->Room.reset();
					}
					Send(nullptr, 0, ct, MessageType::LeaveRoom, SendType::RUdp);
				}
				return true;
			}
			return false;
		}), rooms_.end());
}

/**
 * @brief		�N���C�A���g�̃��[������
 */
void Server::EnterRoom(RoomPtr room, ConnectClientPtr ct) {
	if (room == nullptr)
	{
		SendError(ErrorCode::InvalidRoom, ct->addr, SendType::RUdp);
		return;
	}
	if (room->clients.size() >= room->maxClient)
	{
		SendError(ErrorCode::MaxClientInTheRoom, ct->addr, SendType::RUdp);
		return;
	}
	room->clients.push_back(*ct);
	ct->Room = room;
	SERVER_INFO_LOG_LV2("EnterRoom [" << ct->uid << "] : Member Is [" << room->clients.size() << "]");
	//���[���������[�U�[�ւ̃��[�����̑��M
	for (auto& client : room->clients)
	{
		auto it = GetClient(client.uid);
		SERVER_INFO_LOG_LV3("\tClient [" << client.uid << "] : RoomAddClientSend");
		if (it == nullptr)
		{
			ERROR_LOG("Client [" << client.uid << "] : NotFound");
			continue;
		}
		if (it == ct)
		{
			SendRoom(ct->Room, it, MessageType::EnterRoom, SendType::RUdp);
		}
		else
		{
			SERVER_INFO_LOG_LV3("\tRoomAddClient [" << it->uid << "] : Client [" << ct->uid << "] Enter");
			SendRoom(ct->Room, it, MessageType::RoomAddClient, SendType::RUdp);
		}
	}
}

/**
 * @brief		�N���C�A���g�̃��[���ޏo
 */
void Server::LeaveRoom(ConnectClientPtr ct) {
	//�N���C�A���g�̏����폜
	ct->Room->clients.erase(std::remove_if(ct->Room->clients.begin(), ct->Room->clients.end(),
		[ct](const ClientInformation& it) { return it.uid == ct->uid; }), ct->Room->clients.end());
	SERVER_INFO_LOG_LV2("LeaveRoom [" << ct->uid << "] : Member Is [" << ct->Room->clients.size() << "]");
	//���[���������[�U�[�ւ̃��[�����̑��M
	for (auto& client : ct->Room->clients)
	{
		auto it = GetClient(client.uid);
		SERVER_INFO_LOG_LV3("\tClient [" << client.uid << "] : RoomDeleteClient");
		if (it == nullptr)
		{
			ERROR_LOG("Client [" << client.uid << "] : NotFound");
			continue;
		}
		SERVER_INFO_LOG_LV3("\tRoomDeleteClient [" << it->uid << "] : Client [" << ct->uid << "] Leave");
		SendRoom(ct->Room, it, MessageType::RoomDeleteClient, SendType::RUdp);
	}
	ct->Room.reset();
	Send(nullptr, 0, ct, MessageType::LeaveRoom, SendType::RUdp);
}

/**
 * @brief		���
 */
void Server::Release() {
	if (bEnabled)
	{
		bEnabled = false;
		thread_.join();
	}
	socket_.reset();
}

bool Server::IsClient(const uint32_t uid) {
	std::lock_guard<std::mutex> lock(clientlock_);
	return std::find_if(clients_.begin(), clients_.end(), [&](const ConnectClientPtr& client) {return client->uid == uid; }) != clients_.end();
}

bool Server::IsRoom(const uint32_t uid) {
	std::lock_guard<std::mutex> lock(roomlock_);
	return std::find_if(rooms_.begin(), rooms_.end(), [&](const RoomPtr& room) {return room->uid == uid; }) != rooms_.end();
}

Server::ConnectClientPtr Server::GetClient(const uint32_t uid) {
	std::lock_guard<std::mutex> lock(clientlock_);
	const auto& it = std::find_if(clients_.begin(), clients_.end(), [&](const ConnectClientPtr& client) {return client->uid == uid; });
	if (it == clients_.end())
	{
		return ConnectClientPtr();
	}
	return *it;
}

Server::ConnectClientPtr Server::GetClient(const SocketAddr& addr) {
	std::lock_guard<std::mutex> lock(clientlock_);
	const auto& it = std::find_if(clients_.begin(), clients_.end(), [&](const ConnectClientPtr& client)
		{
			return client->addr.sin_addr.S_un.S_addr == addr.sin_addr.S_un.S_addr && client->addr.sin_port == addr.sin_port;
		}
	);
	if (it == clients_.end())
	{
		return ConnectClientPtr();
	}
	return *it;
}

RoomPtr Server::GetRoom(const uint32_t uid) {
	std::lock_guard<std::mutex> lock(roomlock_);
	const auto& it = std::find_if(rooms_.begin(), rooms_.end(), [&](const RoomPtr& room) {return room->uid == uid; });
	if (it == rooms_.end())
	{
		return RoomPtr();
	}
	return *it;
}