#include	"Client.h"

using namespace Sample;
using namespace Sample::SampleServer;

/**
 * @brief		コンストラクタ
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
 * @brief		デストラクタ
 */
Client::~Client()
{
	Release();
}

/**
 * @brief		生成
 */
bool Client::Create(const ServerConfig& sconfig, const char* name, uint16_t port, ClientEventFunctions func)
{
	//ソケット生成
	socket_ = std::make_shared<UdpSocket>();
	if (!socket_->Create())
	{
		ERROR_LOG("クライアント初期化、ポート生成失敗...");
		return false;
	}

	//ソケットバインド
	if (!socket_->Bind(port))
	{
		ERROR_LOG("クライアント初期化、ソケットバインド失敗...");
		return false;
	}

	//ネットワークのデータを設定
	memset(&serverAddr_, 0, sizeof(SocketAddr));
	serverAddr_.sin_family = AF_INET;
	serverAddr_.sin_port = htons(sconfig.port);
	serverAddr_.sin_addr.s_addr = inet_addr(sconfig.IP.c_str());

	//受信登録
	OnFunc = func;

	//接続メッセージ送信
	ConnectMessage cmsg;
	memset(&cmsg, 0, sizeof(cmsg));
	strcpy(cmsg.name, name);
	cmsg.gameID = sconfig.gameID;
	cmsg.versionID = sconfig.versionID;
	Send(&cmsg, sizeof(ConnectMessage), ServerMessage, SendTarget::Server, 0, MessageType::Connect, SendType::RUdp);
	strcpy(info_.name, name);
	ERROR_LOG("サーバーに接続...");
	return true;
}

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
 * @brief		クライアントリスト要求
 */
void Client::RequestClientList() {
	Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::ClientList, SendType::RUdp);
}


/**
 * @brief		ルームリスト要求
 */
void Client::RequestRoomList() {
	Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::RoomList, SendType::RUdp);
}


/**
 * @brief		ルーム作成要求
 * @param[in]		name			ルーム名
 * @param[in]		maxclient		最大接続人数
 */
void Client::CreateRoom(const char* name, uint32_t maxclient) {
	RoomCreateMessage cmsg;
	memset(&cmsg, 0, sizeof(cmsg));
	strcpy(cmsg.name, name);
	cmsg.maxClient = maxclient;
	Send(&cmsg, sizeof(RoomCreateMessage), ServerMessage, SendTarget::Server, 0, MessageType::CreateRoom, SendType::RUdp);
}

/**
 * @brief		ルーム作成要求
 */
void Client::RandomRoom(const char* name, uint32_t maxclient) {
	RoomCreateMessage cmsg;
	memset(&cmsg, 0, sizeof(cmsg));
	strcpy(cmsg.name, name);
	cmsg.maxClient = maxclient;
	Send(&cmsg, sizeof(RoomCreateMessage), ServerMessage, SendTarget::Server, 0, MessageType::CreateAndEnterRandomRoom, SendType::RUdp);
}

/**
 * @brief		ルーム入室要求
 * @param[in]		uid				入室するルームＩＤ
 */
void Client::EnterRoom(uint32_t uid) {
	Send(&uid, sizeof(uid), ServerMessage, SendTarget::Server, 0, MessageType::EnterRoom, SendType::RUdp);
}

/**
 * @brief		ルーム退出要求
 */
void Client::LeaveRoom() {
	Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::LeaveRoom, SendType::RUdp);
}


/**
 * @brief		動作
 */
void Client::Run() {
	//初期化されていない
	if (socket_ == nullptr)
	{
		return;
	}
	//受信用バッファ
	char buf[BufferMax];
	//受信用ヘッダー
	RUDPMessage::Header header;
	//受信アドレス
	SocketAddr addr;
	//RUDP更新
	RUDP_.Update();
	//受信処理
	while (RUDP_.Receive(buf, BufferMax, header, addr, socket_))
	{
		if (serverAddr_.sin_addr.S_un.S_addr != addr.sin_addr.S_un.S_addr)
		{
			ERROR_LOG("Unknown Receive!!");
			continue;
		}
		ParseMessage(buf, header);
	}
	//接続維持送信
	double nt = FrameTimer::Instance().NowTime();
	if (nt - info_.lastTime >= KeepAliveTime)
	{
		info_.lastTime = nt;
		Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::KeepAlive, SendType::Udp);
	}
}

/**
 * @brief		受信メッセージのパース
 */
void Client::ParseMessage(char* pData, RUDPMessage::Header& header) {
	//メッセージから先頭のヘッダー取り出し
	ServerHeader* h = (ServerHeader*)pData;
	//メッセージごとの処理
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
		//自分のルームなら情報を更新
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
				//自分以外のルームとして一時的に生成
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
			//自分以外のルームとして一時的に生成
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
			//不明なクライアントなためサーバーから切断
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
 * @brief		受信メッセージのパース
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
 * @brief		受信メッセージのパース
 */
void Client::ParseRoomMessage(char* pData, RUDPMessage::Header& header, RoomPtr room) {
	//ルームがない
	if (room == nullptr)
	{
		return;
	}
	uint32_t offset = sizeof(ServerHeader);
	//データを取り出していく
	memcpy(&room->uid, &pData[offset], sizeof(room->uid));
	offset += sizeof(room->uid);
	memcpy(room->name, &pData[offset], sizeof(room->name));
	offset += sizeof(room->name);
	memcpy(&room->maxClient, &pData[offset], sizeof(room->maxClient));
	offset += sizeof(room->maxClient);
	//所属クライアント情報
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
	//ルーム情報
	memcpy(&size, &pData[offset], sizeof(size));
	offset += sizeof(size);
	room->roomData.resize(size);
	for (auto& data : room->roomData)
	{
	}
}

/**
 * @brief		受信メッセージのパース
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
 * @brief		解放
 */
void Client::Release() {
	//切断メッセージ送信
	if (socket_ != nullptr)
	{
		Send(nullptr, 0, ServerMessage, SendTarget::Server, 0, MessageType::DisConnect, SendType::Udp);
	}
	socket_.reset();
	info_.uid = 0;
}