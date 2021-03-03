#pragma once

#include	"NetworkScene.h"
#include	"CellPosition.h"
#include	"PushMove.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
NetworkScene::NetworkScene()
: Stage_()
, Characters_()
, Clear_()
, config("127.0.0.1", 18000, 1, 1) 
, server()
, client() {
	Name_ = "Network";
}

/**
 * @brief		デストラクタ
 */
NetworkScene::~NetworkScene() {
}

/**
 * @brief		継承先のシーンの構築をおこなう
 */
void NetworkScene::OnCreate() {
	//テクスチャ追加
	Resources_->Textures().Add("chip", "chip.png");
	Resources_->Textures().Add("chara", "chara.png");
	Resources_->Textures().Add("clear", "clear.png");

	//アクター登録
	//ステージ
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Stage");
		auto st = pt->AddParameter<Stage>();
		st->SetNetwork(true);
	}
	//クリア
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Clear");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("clear"), ResourceManager::Instance().GetShader("Sprite"));
		rd->Priority(2);
		rd->Anchor(Vector3F(0.5f, 0.5f, 0.5f));
		rd->Pivot(Vector3F(0.5f, 0.5f, 0.5f));
		pt->SetActive(false);
	}
	//地面
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Ground");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(64, 0, 96, 32));
		pt->AddParameter<CellPosition>();
		pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
	}
	//壁
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Wall");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(0, 32, 32, 64));
		pt->AddParameter<CellPosition>();
		pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
	}
	//荷物を納める場所
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Target");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(128, 0, 160, 32));
		pt->AddParameter<CellPosition>();
		pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
	}
	//荷物
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Object");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(32, 64, 64, 96));
		pt->AddParameter<CellPosition>();
		pt->AddAction<PushMove>();
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
		rd->Priority(1);
	}
	//キャラクター
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Player");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->Register("Up",
			{ true,
				{
					{0.2f, RectangleF(0,0,32,32)},
					{0.2f, RectangleF(32,0,64,32)},
					{0.2f, RectangleF(64,0,96,32)}
				}
			});
		anim->Register("Right",
			{ true,
				{
					{0.2f, RectangleF(0,32,32,64)},
					{0.2f, RectangleF(32,32,64,64)},
					{0.2f, RectangleF(64,32,96,64)}
				}
			});
		anim->Register("Down",
			{ true,
				{
					{0.2f, RectangleF(0,64,32,96)},
					{0.2f, RectangleF(32,64,64,96)},
					{0.2f, RectangleF(64,64,96,96)}
				}
			});
		anim->Register("Left",
			{ true,
				{
					{0.2f, RectangleF(0,96,32,128)},
					{0.2f, RectangleF(32,96,64,128)},
					{0.2f, RectangleF(64,96,96,128)}
				}
			});
		anim->ChangeAnimation("Up");
		pt->AddAction<NetworkKeyMove>();
		pt->AddParameter<CellPosition>();
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("chara"), ResourceManager::Instance().GetShader("Sprite"));
		rd->Priority(1);
	}

	ActorPtr pt = Instaniate("Stage");
	Stage_ = pt->GetParameter<Stage>();
	Clear_ = Instaniate("Clear");
}

/**
 * @brief		継承先のシーンの更新処理をおこなう
 */
void NetworkScene::OnUpdate() {
	if (server == nullptr && client == nullptr)
	{
		//クライアント機能開始
		if (InputManager::Instance().IsPush("Left") || InputManager::Instance().IsPush("Right"))
		{
			client = std::make_shared<SampleServer::Client>();
			SampleServer::IClient::ClientEventFunctions func;
			func.eventInterface = this;
			//左右キーでIDを変えて開始
			if (InputManager::Instance().IsPush("Left"))
			{
				client->Create(config, "client1", 19000, func);
			}
			else if (InputManager::Instance().IsPush("Right"))
			{
				client->Create(config, "client2", 19001, func);
			}
		}
		//サーバー機能開始
		if (InputManager::Instance().IsPush("Down"))
		{
			server = std::make_shared<SampleServer::Server>();
			server->Create(config);
		}
	}
	//更新
	if (client)
	{
		client->Run();
	}
}

/**
 * @brief		継承先のシーンの更新後処理をおこなう
 */
void NetworkScene::OnLateUpdate() {
	if (Stage_->IsClear())
	{
		Clear_->SetActive(true);
	}
}

/**
 * @brief		信号の受信処理
 * @param[in]	sheader			受信したサーバー情報
 * @param[in]	pData			受信したデータ
 * @param[in]	header			受信したデータヘッダー
 */
void NetworkScene::OnReceive(const SampleServer::ServerHeader& sheader, void* pData, const RUDPMessage::Header& header)
{
	//キャラクターを検索
	auto it = std::find_if(Characters_.begin(), Characters_.end(), [sheader](const NetworkKeyMovePtr& mv)
		{ return mv->GetOwnerID() == sheader.senduid; });
	//キャラクターに移動情報を適用
	if (it != Characters_.end())
	{
		int x, y;
		char* buf = (char*)pData;
		memcpy(&x, &buf[0], sizeof(int));
		memcpy(&y, &buf[4], sizeof(int));
		(*it)->Move(x, y);
	}
}

/**
 * @brief		サーバーに接続成功処理
 */
void NetworkScene::OnConnect()
{
	//とりあえずランダムなルームへ入室
	client->RandomRoom("room1", 2);
}

/**
 * @brief		ルーム作成成功
 */
void NetworkScene::OnRoomCreate()
{
	//自分のキャラクターを生成
	ActorPtr pt = Instaniate("Player");
	//ネットワークのキー処理をコンポーネントとして追加
	auto mkey = pt->GetAction<NetworkKeyMove>();
	mkey->SetClient(client);
	mkey->SetOwnerID(client->GetUID());
	pt->GetParameter<CellPosition>()->Position(strcmp("client1", client->GetName()) == 0 ? 1 : 11, 1, 0);
	Characters_.push_back(mkey);
}

/**
 * @brief		エラー発生時の処理
 * @param[in]	code			発生したエラーコード
 */
void NetworkScene::OnError(SampleServer::ErrorCode code)
{
}

/**
 * @brief		クライアントリスト受信時の処理
 * @param[in]	list			受信したクライアントリスト
 */
void NetworkScene::OnClientList(std::vector<SampleServer::ClientListMessage>& list)
{
	//接続中のクライアントリストを表示
	for (auto& it : list)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
	}
}

/**
 * @brief		ルームリスト受信時の処理
 * @param[in]	list			受信したルームリスト
 */
void NetworkScene::OnRoomList(std::vector<SampleServer::RoomListMessage>& list)
{
	//接続中のルームリストを表示
	for (auto& it : list)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
	}
	//とりあえず先頭のルームに入室
	client->EnterRoom(list[0].uid);
}

/**
 * @brief		ルーム入室時の処理
 */
void NetworkScene::OnEnterRoom()
{
	//入室したルームIDを表示
	std::cout << "Enter [ " << client->GetRoom()->uid << "]" << std::endl;
	//ルーム内のクライアントに対してテストの表示とクライアントの操作キャラクタを生成
	for (auto& it : client->GetRoom()->clients)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
		ActorPtr pt = Instaniate("Player");
		auto mkey = pt->GetAction<NetworkKeyMove>();
		mkey->SetClient(client);
		mkey->SetOwnerID(it.uid);
		pt->GetParameter<CellPosition>()->Position(strcmp("client1", it.name) == 0 ? 1 : 11, 1, 0);
		Characters_.push_back(mkey);
	}
}

/**
 * @brief		ルーム退出時の処理
 */
void NetworkScene::OnLeaveRoom()
{
}

/**
 * @brief		ルームに新しいクライアントが入ってきた時の処理
 */
void NetworkScene::OnRoomAddClient()
{
	//入室してきたクライアントIDを表示
	std::cout << "AddClient [ " << client->GetRoom()->uid << "]" << std::endl;
	//ルーム内のクライアントに対してテストの表示とクライアントの操作キャラクタを生成
	for (auto& it : client->GetRoom()->clients)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
		//クライアントの操作キャラクタを検索していない場合のみ生成
		if (std::find_if(Characters_.begin(), Characters_.end(), [it](const NetworkKeyMovePtr& mv)
			{ return mv->GetOwnerID() == it.uid; }) == Characters_.end())
		{
			ActorPtr pt = Instaniate("Player");
			auto mkey = pt->GetAction<NetworkKeyMove>();
			mkey->SetClient(client);
			mkey->SetOwnerID(it.uid);
			pt->GetParameter<CellPosition>()->Position(strcmp("client1", it.name) == 0 ? 1 : 11, 1, 0);
			Characters_.push_back(mkey);
		}
	}
}

/**
 * @brief		ルームからクライアントが退出した時の処理
 */
void NetworkScene::OnRoomDeleteClient()
{
	std::cout << "DeleteClient [ " << client->GetRoom()->uid << "]" << std::endl;
	for (auto& it : client->GetRoom()->clients)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
	}
}

/**
 * @brief		ルーム情報受信時の処理
 * @param[in]	room			受信したルーム情報
 */
void NetworkScene::OnRoomInformation(const SampleServer::RoomPtr& room)
{
}
