#pragma once

#include	"Scene.h"

namespace SceneSample {
	/**
	 * @brief		ゲームシーン
	 */
	class GameScene : public IScene {
	private:
	public:
		/**
		 * @brief		ゲームシーン初期化用構造体
		 *				ゲームシーンの初期化に必要な情報があれば設定する
		 */
		struct InitializeParam : public SceneInitializeParam {
		};

		/**
		 * @brief		コンストラクタ
		 */
		GameScene() {
		}

		/**
		 * @brief		コンストラクタ
		 */
		virtual ~GameScene() {
		}

		/**
		 * @brief		初期化
		 * @param[in]	param		初期化情報
		 * @return		true		初期化正常終了
		 *				false		初期化失敗
		 */
		bool Initialize(const SceneInitializeParamPointer& param) override {
			//ゲームの初期化情報として取り出し
			auto inparam = std::dynamic_pointer_cast<InitializeParam>(param);
			//テスト用表示
			std::cout << "----------------------------------------" << std::endl;
			std::cout << "ゲーム初期化..." << std::endl;
			return true;
		}

		/**
		 * @brief		更新
		 */
		void Update() override {
			//テスト用表示
			std::cout << "ゲーム更新..." << std::endl;
			std::cout << "1...タイトル画面" << std::endl;
			//入力キーの取得
			int key = _getch();
			if (key == '1')
			{
				Notify("Title", nullptr);
			}
		}

		/**
		 * @brief		描画
		 */
		void Render() override {
			//テスト用表示
			std::cout << "ゲーム描画..." << std::endl;
		}

		/**
		 * @brief		破棄
		 */
		void Destroy() override {
			//テスト用表示
			std::cout << "ゲーム破棄..." << std::endl;
			std::cout << "----------------------------------------\n" << std::endl;
		}
	};
}