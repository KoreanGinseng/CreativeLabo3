#pragma once

#include	"Stage.h"

namespace Sample {

	/**
	 * @brief		ゲームメインシーンの定義
	 */
	class GameScene : public Scene {
	private:
		StagePtr		Stage_;
		ActorPtr		Clear_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		GameScene();

		/**
		 * @brief		デストラクタ
		 */
		virtual ~GameScene();

		/**
		 * @brief		継承先のシーンの構築をおこなう
		 */
		void OnCreate() override;

		/**
		 * @brief		継承先のシーンの更新処理をおこなう
		 */
		void OnUpdate() override;

		/**
		 * @brief		継承先のシーンの更新後処理をおこなう
		 */
		void OnLateUpdate() override;

	};
}