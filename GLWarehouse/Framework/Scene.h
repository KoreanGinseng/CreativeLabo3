#pragma once

#include	"World.h"
#include	"FrameBuffer.h"
#include	"Sprite.h"
#include	"TextureRender.h"
#include	"RenderManager.h"
#include	"ResourceManager.h"

namespace Sample {

	class Scene : public World
	{
	protected:
		/** シーンの名前 */
		std::string									Name_;

		/** シーン内のメインカメラ */
		CameraPtr									CurrentCamera_;
		
		/** シーンのレンダリング結果 */
		FrameBufferPtr								FrameBuffer_;

		/** フレーム描画用スプライト */
		SpritePtr									FrameSprite_;

		/** シーン内の利用リソース */
		ResourcePackPtr								Resources_;

		/** 更新フラグ */
		bool										bUpdate_;

		/** 終了フラグ */
		bool										bEnd_;

		Scene(const Scene& obj) = delete;
		Scene& operator=(const Scene&) = delete;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		Scene()
		: World() 
		, Name_()
		, CurrentCamera_() 
		, FrameBuffer_()
		, FrameSprite_()
		, Resources_()
		, bUpdate_(true) 
		, bEnd_(false) {
		}

		/**
		 * @brief		デストラクタ
		 */
		virtual ~Scene() {
			Release();
		}

		/**
		 * @brief		シーンの構築をおこなう
		 */
		virtual void Create() {
			//接触初期化
			Collisions_.Initialize({0xFFFFFFFF});

			//リソースパック生成
			Resources_ = ResourceManager::Instance().AddPack(Name_);

			//フレームバッファ生成
			FrameBuffer_ = std::make_shared<FrameBuffer>();
			FrameBuffer_->Create(RenderManager::Instance().ScreenWidth(),
				RenderManager::Instance().ScreenHeight());
			FrameSprite_ = std::make_shared<Sprite>();
			FrameSprite_->YFlip(true);
			FrameSprite_->Create(FrameBuffer_->Color(), ResourceManager::Instance().GetShader("Sprite"));
			FrameSprite_->RefreshMatrix();

			//アクターリスト生成
			Actors_ = std::make_shared<ActorList>();
			ActionActors_ = std::make_shared<ActorList>();

			//カメラ生成
			CurrentCamera_ = std::make_shared<Camera>();
			CurrentCamera_->Create2D(FrameBuffer_->Width(), FrameBuffer_->Height());
			Actors_->push_back(CurrentCamera_);
			RootActors_.push_back(CurrentCamera_);

			//継承先のシーンの構築をおこなう
			OnCreate();
		}

		/**
		 * @brief		継承先のシーンの構築をおこなう
		 */
		virtual void OnCreate() {
		}

		/**
		 * @brief		シーンの更新をおこなう
		 */
		virtual void Update() {
			//更新なし
			if (!bUpdate_)
			{
				return;
			}

			//更新処理
			OnUpdate();

			//アクター開始
			if (StartActors_.size() > 0)
			{
				std::for_each(StartActors_.begin(), StartActors_.end(), [&](const ActorPtr& ptr) { ptr->Start(); });
				StartActors_.clear();
			}

			//アクター更新
			for (auto it : RootActors_)
			{
				if (!it->IsActive())
				{
					continue;
				}
				it->Update();
			}

			//接触判定実行
			Collisions_.Collision();

			//更新後処理
			OnLateUpdate();

			//終了アクターの消去
			Actors_->erase(std::remove_if(Actors_->begin(), Actors_->end(),
				[](const ActorPtr& act) { return act->IsEnd(); }), Actors_->end());
			RootActors_.erase(std::remove_if(RootActors_.begin(), RootActors_.end(),
				[](const ActorPtr& act) { return act->IsEnd(); }), RootActors_.end());
			ActionActors_->erase(std::remove_if(ActionActors_->begin(), ActionActors_->end(),
				[](const ActorPtr& act) { return act->IsEnd(); }), ActionActors_->end());
		}

		/**
		 * @brief		継承先のシーンの更新処理をおこなう
		 */
		virtual void OnUpdate() {
		}

		/**
		 * @brief		継承先のシーンの更新後処理をおこなう
		 */
		virtual void OnLateUpdate() {
		}

		/**
		 * @brief		シーンの描画をおこなう
		 */
		virtual void Render() {
			//シーン内にカメラが設定されていない
			if (!CurrentCamera_)
			{
				CurrentCamera_ = FindActor<Camera>();
				if (!CurrentCamera_)
				{
					ERROR_LOG("Camera Empty!!");
				}
				return;
			}
			//シーンのカメラを設定
			RenderManager::Instance().Camera(CurrentCamera_);
			//フレームバッファ利用
			FrameBuffer_->Bind();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//描画の実行
			for (auto it : Renders_)
			{
				for (auto ra : it.Renders)
				{
					if (!ra->GetActor()->IsActive())
					{
						continue;
					}
					if (!ra->Enabled())
					{
						continue;
					}
					ra->Render();
				}
				it.Renders.erase(std::remove_if(it.Renders.begin(), it.Renders.end(),
					[](const RenderPtr& act) { return act->GetActor()->IsEnd(); }), it.Renders.end());
			}
			//デフォルトに戻す
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glViewport(0, 0, FrameBuffer_->Width(), FrameBuffer_->Height());
		}
		/**
		 * @brief		シーンの表示
		 */
		virtual void Present() {
			//フレームの描画
			FrameSprite_->Render();
		}

		/**
		 * @brief		プロダクトからアクターを生成
		 * @param[in]	key		キー
		 * @param[in]	pos		生成位置
		 * @return		生成したアクター
		 */
		ActorPtr Instaniate(const std::string& key, const Vector3F& pos = Vector3F())
		{
			ActorPtr re = Resources_->GetActorFactory().Create(key);
			if (re == nullptr)
			{
				return ActorPtr();
			}
			re->Position(pos);
			AddActor(re);
			return re;
		}

		/**
		 * @brief		このシーンのリソースの取得
		 * @return		シーンのリソース
		 */
		ResourcePackPtr& CurrentResource() {
			return Resources_;
		}

		/**
		 * @brief		利用中のカメラの設定
		 * @param[in]	cam		利用中のカメラ
		 */
		void CurrentCamera(CameraPtr& cam) {
			CurrentCamera_ = cam;
		}

		/**
		 * @brief		利用中のカメラの取得
		 * @return		利用中のカメラ
		 */
		const CameraPtr& CurrentCamera() const {
			return CurrentCamera_;
		}

		/**
		 * @brief		シーンスプライトの取得
		 * @return		シーンスプライト
		 */
		const SpritePtr& FrameSprite() const {
			return FrameSprite_;
		}

		/**
		 * @brief		終了フラグを立てる
		 */
		void Destroy() { bEnd_ = true; }

		/**
		 * @brief		終了判定
		 * @return		終了フラグ
		 */
		bool IsEnd() const { return bEnd_; }

		/**
		 * @brief		ワールドの参照をなくす
		 */
		virtual void Release() {
			World::Release();
			ResourceManager::Instance().DeletePack(Name_);
			Resources_.reset();
			CurrentCamera_.reset();
			FrameBuffer_.reset();
			FrameSprite_.reset();
		}

		/**
		 * @brief		シーンの保存
		 */
		bool Save();

		/**
		 * @brief		シーンの読み込み
		 */
		bool Load();
	};
	using ScenePtr = std::shared_ptr< Scene >;
	using SceneList = std::vector<ScenePtr>;

}