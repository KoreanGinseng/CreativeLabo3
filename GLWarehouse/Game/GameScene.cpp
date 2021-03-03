#pragma once

#include	"GameScene.h"
#include	"Stage.h"
#include	"CellPosition.h"
#include	"KeyMove.h"
#include	"PushMove.h"

using namespace Sample;

/**
 * @brief		コンストラクタ
 */
GameScene::GameScene() {
	Name_ = "Game";
}

/**
 * @brief		デストラクタ
 */
GameScene::~GameScene() {
}

/**
 * @brief		継承先のシーンの構築をおこなう
 */
void GameScene::OnCreate() {
	//テクスチャ追加
	Resources_->Textures().Add("chip", "chip.png");
	Resources_->Textures().Add("chara", "chara.png");
	Resources_->Textures().Add("clear", "clear.png");

	//アクター登録
	//ステージ
	{
		ActorPtr pt = Resources_->GetActorFactory().CreateProduct("Stage");
		pt->AddParameter<Stage>();
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
		pt->AddAction<KeyMove>();
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
void GameScene::OnUpdate() {
}

/**
 * @brief		継承先のシーンの更新後処理をおこなう
 */
void GameScene::OnLateUpdate() {
	if (Stage_->IsClear())
	{
		Clear_->SetActive(true);
	}
}