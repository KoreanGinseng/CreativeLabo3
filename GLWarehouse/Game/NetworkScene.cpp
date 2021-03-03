#pragma once

#include	"NetworkScene.h"
#include	"CellPosition.h"
#include	"PushMove.h"

using namespace Sample;

/**
 * @brief		�R���X�g���N�^
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
 * @brief		�f�X�g���N�^
 */
NetworkScene::~NetworkScene() {
}

/**
 * @brief		�p����̃V�[���̍\�z�������Ȃ�
 */
void NetworkScene::OnCreate() {
	//�e�N�X�`���ǉ�
	Resources_->Textures().Add("chip", "chip.png");
	Resources_->Textures().Add("chara", "chara.png");
	Resources_->Textures().Add("clear", "clear.png");

	//�A�N�^�[�o�^
	//�X�e�[�W
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Stage");
		auto st = pt->AddParameter<Stage>();
		st->SetNetwork(true);
	}
	//�N���A
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Clear");
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("clear"), ResourceManager::Instance().GetShader("Sprite"));
		rd->Priority(2);
		rd->Anchor(Vector3F(0.5f, 0.5f, 0.5f));
		rd->Pivot(Vector3F(0.5f, 0.5f, 0.5f));
		pt->SetActive(false);
	}
	//�n��
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Ground");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(64, 0, 96, 32));
		pt->AddParameter<CellPosition>();
		pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
	}
	//��
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Wall");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(0, 32, 32, 64));
		pt->AddParameter<CellPosition>();
		pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
	}
	//�ו���[�߂�ꏊ
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Target");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(128, 0, 160, 32));
		pt->AddParameter<CellPosition>();
		pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
	}
	//�ו�
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Object");
		auto anim = pt->AddAction<SpriteAnimator>();
		anim->SrcRect(RectangleF(32, 64, 64, 96));
		pt->AddParameter<CellPosition>();
		pt->AddAction<PushMove>();
		auto rd = pt->AddRender<TextureRender>(Resources_->GetTexture("chip"), ResourceManager::Instance().GetShader("Sprite"));
		rd->Priority(1);
	}
	//�L�����N�^�[
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
 * @brief		�p����̃V�[���̍X�V�����������Ȃ�
 */
void NetworkScene::OnUpdate() {
	if (server == nullptr && client == nullptr)
	{
		//�N���C�A���g�@�\�J�n
		if (InputManager::Instance().IsPush("Left") || InputManager::Instance().IsPush("Right"))
		{
			client = std::make_shared<SampleServer::Client>();
			SampleServer::IClient::ClientEventFunctions func;
			func.eventInterface = this;
			//���E�L�[��ID��ς��ĊJ�n
			if (InputManager::Instance().IsPush("Left"))
			{
				client->Create(config, "client1", 19000, func);
			}
			else if (InputManager::Instance().IsPush("Right"))
			{
				client->Create(config, "client2", 19001, func);
			}
		}
		//�T�[�o�[�@�\�J�n
		if (InputManager::Instance().IsPush("Down"))
		{
			server = std::make_shared<SampleServer::Server>();
			server->Create(config);
		}
	}
	//�X�V
	if (client)
	{
		client->Run();
	}
}

/**
 * @brief		�p����̃V�[���̍X�V�㏈���������Ȃ�
 */
void NetworkScene::OnLateUpdate() {
	if (Stage_->IsClear())
	{
		Clear_->SetActive(true);
	}
}

/**
 * @brief		�M���̎�M����
 * @param[in]	sheader			��M�����T�[�o�[���
 * @param[in]	pData			��M�����f�[�^
 * @param[in]	header			��M�����f�[�^�w�b�_�[
 */
void NetworkScene::OnReceive(const SampleServer::ServerHeader& sheader, void* pData, const RUDPMessage::Header& header)
{
	//�L�����N�^�[������
	auto it = std::find_if(Characters_.begin(), Characters_.end(), [sheader](const NetworkKeyMovePtr& mv)
		{ return mv->GetOwnerID() == sheader.senduid; });
	//�L�����N�^�[�Ɉړ�����K�p
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
 * @brief		�T�[�o�[�ɐڑ���������
 */
void NetworkScene::OnConnect()
{
	//�Ƃ肠���������_���ȃ��[���֓���
	client->RandomRoom("room1", 2);
}

/**
 * @brief		���[���쐬����
 */
void NetworkScene::OnRoomCreate()
{
	//�����̃L�����N�^�[�𐶐�
	ActorPtr pt = Instaniate("Player");
	//�l�b�g���[�N�̃L�[�������R���|�[�l���g�Ƃ��Ēǉ�
	auto mkey = pt->GetAction<NetworkKeyMove>();
	mkey->SetClient(client);
	mkey->SetOwnerID(client->GetUID());
	pt->GetParameter<CellPosition>()->Position(strcmp("client1", client->GetName()) == 0 ? 1 : 11, 1, 0);
	Characters_.push_back(mkey);
}

/**
 * @brief		�G���[�������̏���
 * @param[in]	code			���������G���[�R�[�h
 */
void NetworkScene::OnError(SampleServer::ErrorCode code)
{
}

/**
 * @brief		�N���C�A���g���X�g��M���̏���
 * @param[in]	list			��M�����N���C�A���g���X�g
 */
void NetworkScene::OnClientList(std::vector<SampleServer::ClientListMessage>& list)
{
	//�ڑ����̃N���C�A���g���X�g��\��
	for (auto& it : list)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
	}
}

/**
 * @brief		���[�����X�g��M���̏���
 * @param[in]	list			��M�������[�����X�g
 */
void NetworkScene::OnRoomList(std::vector<SampleServer::RoomListMessage>& list)
{
	//�ڑ����̃��[�����X�g��\��
	for (auto& it : list)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
	}
	//�Ƃ肠�����擪�̃��[���ɓ���
	client->EnterRoom(list[0].uid);
}

/**
 * @brief		���[���������̏���
 */
void NetworkScene::OnEnterRoom()
{
	//�����������[��ID��\��
	std::cout << "Enter [ " << client->GetRoom()->uid << "]" << std::endl;
	//���[�����̃N���C�A���g�ɑ΂��ăe�X�g�̕\���ƃN���C�A���g�̑���L�����N�^�𐶐�
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
 * @brief		���[���ޏo���̏���
 */
void NetworkScene::OnLeaveRoom()
{
}

/**
 * @brief		���[���ɐV�����N���C�A���g�������Ă������̏���
 */
void NetworkScene::OnRoomAddClient()
{
	//�������Ă����N���C�A���gID��\��
	std::cout << "AddClient [ " << client->GetRoom()->uid << "]" << std::endl;
	//���[�����̃N���C�A���g�ɑ΂��ăe�X�g�̕\���ƃN���C�A���g�̑���L�����N�^�𐶐�
	for (auto& it : client->GetRoom()->clients)
	{
		std::cout << "[" << it.uid << "]" << it.name << std::endl;
		//�N���C�A���g�̑���L�����N�^���������Ă��Ȃ��ꍇ�̂ݐ���
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
 * @brief		���[������N���C�A���g���ޏo�������̏���
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
 * @brief		���[������M���̏���
 * @param[in]	room			��M�������[�����
 */
void NetworkScene::OnRoomInformation(const SampleServer::RoomPtr& room)
{
}
