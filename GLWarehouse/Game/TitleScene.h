#pragma once

#include	"Stage.h"

namespace Sample {

	/**
	 * @brief		タイトルシーンの定義
	 */
	class TitleScene : public Scene {
	private:
	public:
		/**
		 * @brief		コンストラクタ
		 */
		TitleScene();

		/**
		 * @brief		デストラクタ
		 */
		virtual ~TitleScene();

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