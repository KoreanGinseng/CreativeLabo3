#pragma once

#include	<unordered_map>
#include	"Scene.h"
#include	"Singleton.h"
#include	"Observer.h"

namespace SceneSample {
	/**
	 * シーンマネージャー
	 */
	class SceneManager : public Singleton<SceneManager>, public Observer<const KeyType&, const SceneInitializeParamPointer&>
	{
		friend class Singleton<SceneManager>;
	public:
		/**
		 * シーン生成用のインターフェイス
		 */
		class ISceneCreator {
		public:
			virtual ScenePointer Create() = 0;
			virtual SceneInitializeParamPointer CreateInitializeParam() = 0;
		};
		/**
		 * シーン生成用のクラス
		 */
		template < class T >
		class SceneCreator : public ISceneCreator {
		public:
			ScenePointer Create() override { return std::make_shared<T>(); }
			SceneInitializeParamPointer CreateInitializeParam() override { return std::make_shared<T::InitializeParam>(); }
		};
	private:
		/** 実行中のシーン */
		ScenePointer scene;

		/** 登録されているシーン */
		std::unordered_map<KeyType, std::unique_ptr< ISceneCreator > > creator;

		/**
		 * @brief		コンストラクタ
		 */
		SceneManager() 
		: Singleton<SceneManager> ()
		, scene()
		, creator() { }
	public:
		/**
		 * @brief		シーンの登録
		 * @param[in]	key			登録キー
		 */
		template < class T >
		SceneManager& Register(const KeyType& key) {
			creator[key] = std::make_unique<SceneCreator<T>>();
			return *this;
		}

		/**
		 * @brief		シーン変更
		 * @param[in]	key			登録キー
		 * @return		true		遷移成功
		 *				false		遷移失敗
		 */
		bool ChangeScene(const KeyType& key) {
			return ChangeScene(key, creator[key]->CreateInitializeParam());
		}

		/**
		 * @brief		シーン変更
		 * @param[in]	key			登録キー
		 * @param[in]	param		初期化情報
		 * @return		true		遷移成功
		 *				false		遷移失敗
		 */
		bool ChangeScene(const KeyType& key, const SceneInitializeParamPointer& param) {
			//新しいシーンを作成
			ScenePointer temp = creator[key]->Create();
			temp->Subscribe(this);
			//新しいシーンを初期化
			if (!temp->Initialize(param))
			{
				return false;
			}
			//古いシーンを破棄
			if (scene) { scene->Destroy(); }
			//新しいシーンを代入
			scene = temp;
			return true;
		}

		/**
		 * @brief		更新
		 */
		void Update()
		{
			if (scene) { scene->Update(); }
		}

		/**
		 * @brief		描画
		 */
		void Render()
		{
			if (scene) { scene->Render(); }
		}

		/**
		 * @brief	通知メソッド
		 */
		void Notify(const KeyType& key, const SceneInitializeParamPointer& param) override {
			ChangeScene(key, param);
		}
	};
}
//簡易アクセス用
#define SceneManagerInstance	SceneSample::SceneManager::GetInstance()