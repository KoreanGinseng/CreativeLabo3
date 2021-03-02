#pragma once

//INCLUDE
#include		<iostream>
#include		<memory>
#include		<conio.h>		//_kbhit、_getchを使うために追加
#include		"Subject.h"

namespace SceneSample {
	//シーンキー(enum classを利用する場合変更)
	using KeyType = std::string;

	/**
	 * @brief		シーン初期化用構造体
	 *				継承先のシーンで継承して実体を作成する
	 */
	struct SceneInitializeParam {
		virtual ~SceneInitializeParam() {}
	};
	using SceneInitializeParamPointer = std::shared_ptr<SceneInitializeParam>;

	/**
	 * @brief		シーン用インターフェイス
	 */
	class IScene : public Subject<const KeyType&, const SceneInitializeParamPointer&> {
	public:

		/**
		 * @brief		初期化
		 * @param[in]	param		初期化情報
		 * @return		true		初期化正常終了
		 *				false		初期化失敗
		 */
		virtual bool Initialize(const SceneInitializeParamPointer& param) = 0;

		/**
		 * @brief		更新
		 */
		virtual void Update() = 0;

		/**
		 * @brief		描画
		 */
		virtual void Render() = 0;

		/**
		 * @brief		破棄
		 */
		virtual void Destroy() = 0;
	};
	using ScenePointer = std::shared_ptr<IScene>;
}