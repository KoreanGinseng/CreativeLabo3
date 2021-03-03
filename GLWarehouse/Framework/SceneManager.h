#pragma once

#include	"Scene.h"
#include	"SceneChangeEffect.h"

namespace Sample {

	class SceneManager
	{
	private:
		SceneList				Scenes_;
		ScenePtr				CurrentScene_;
		SceneChangePtr			Change_;

		SceneManager()
		: Scenes_() 
		, CurrentScene_() 
		, Change_() {
		}
		SceneManager(const SceneManager& obj) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		virtual ~SceneManager() = default;
	public:

		/**
		 * @brief		シーンの生成
		 */
		template <typename T>
		void CreateScene() {
			CurrentScene_ = std::make_shared<T>();
			CurrentScene_->Create();
			Scenes_.push_back(CurrentScene_);
		}

		/**
		 * @brief		シーンの生成
		 */
		template <typename T>
		void LoadScene() {
			CurrentScene_ = std::make_shared<T>();
			CurrentScene_->Create();
			CurrentScene_->Load();
			Scenes_.push_back(CurrentScene_);
		}

		/**
		 * @brief		シーンの生成
		 */
		template <typename T>
		void ChangeNewScene(const SceneChangeEffectPtr Out, const SceneChangeEffectPtr In) {
			CurrentScene_ = std::make_shared<T>();
			CurrentScene_->Create();
			Change_ = std::make_shared<SceneChange>(Scenes_, CurrentScene_, Out, In);
			Scenes_.push_back(CurrentScene_);
		}

		/**
		 * @brief		シーンの生成
		 */
		template <typename T>
		void ChangeLoadNewScene(const SceneChangeEffectPtr Out, const SceneChangeEffectPtr In) {
			CurrentScene_ = std::make_shared<T>();
			CurrentScene_->Create();
			CurrentScene_->Load();
			Change_ = std::make_shared<SceneChange>(Scenes_, CurrentScene_, Out, In);
			Scenes_.push_back(CurrentScene_);
		}

		/**
		 * @brief		更新処理
		 */
		void Update() {
			//遷移中
			if (Change_)
			{
				Change_->Update();
				if (Change_->IsEnd())
				{
					Change_->Release();
					Change_.reset();
				}
			}
			//シーン更新
			std::for_each(Scenes_.begin(), Scenes_.end(), [&](const ScenePtr& ptr) {CurrentScene_ = ptr; ptr->Update(); });
			//終了シーンは取り除く
			Scenes_.erase(std::remove_if(Scenes_.begin(), Scenes_.end(),
				[](const ScenePtr& sce) { return sce->IsEnd(); }), Scenes_.end());
		}

		/**
		 * @brief		描画処理
		 */
		void Render() {
			//描画
			std::for_each(Scenes_.begin(), Scenes_.end(), [&](const ScenePtr& ptr) { CurrentScene_ = ptr; ptr->Render(); });
			//遷移中
			if (Change_)
			{
				Change_->Render();
				return;
			}
			std::for_each(Scenes_.begin(), Scenes_.end(), [](const ScenePtr& ptr) { ptr->Present(); });
		}


		/**
		 * @brief		解放処理
		 */
		void Release() {
			std::for_each(Scenes_.begin(), Scenes_.end(), [&](const ScenePtr& ptr) {CurrentScene_ = ptr; ptr->Release(); });
			Scenes_.clear();
			CurrentScene_.reset();
		}

		/**
		 * @brief		プロダクトからアクターを生成
		 * @param[in]	key		キー
		 * @param[in]	pos		生成位置
		 * @return		生成したアクター
		 */
		ActorPtr Instaniate(const std::string& key, const Vector3F& pos = Vector3F())
		{
			if (CurrentScene_ == nullptr)
			{
				return ActorPtr();
			}
			return CurrentScene_->Instaniate(key, pos);
		}

		/**
		 * @brief		現在のシーン取得
		 */
		ScenePtr& CurrentScene() {
			return CurrentScene_;
		}

		static SceneManager& Instance() {
			static SceneManager t;
			return t;
		}
	};

}