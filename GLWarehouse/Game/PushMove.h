#pragma once

#include	"Stage.h"

namespace Sample {

	class PushMove : public ActionBase<PushMove> {
	private:
		Vector3F Target_;
		CellPositionPtr CellPosition_;
		StagePtr Stage_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		PushMove()
		: ActionBase() 
		, Target_()
		, CellPosition_()
		, Stage_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		PushMove(const PushMove& obj)
		: ActionBase(obj) 
		, Target_()
		, CellPosition_()
		, Stage_() {
		}

		/**
		 * @brief		デストラクタ
		 */
		~PushMove() {
		}

		/**
		 * @brief		開始
		 */
		void Start() override {
			CellPosition_ = actor->GetParameter<CellPosition>();
			Target_ = CellPosition_->ScreenPosition();
			Stage_ = SceneManager::Instance().CurrentScene()->FindParameter<Stage>();
		}

		/**
		 * @brief		更新
		 */
		void Update() override {
			//アクターの座標を取得
			Vector3F pos = actor->Position();

			//経過時間 ※UnityのTime.deltaTimeに対応
			float t = FrameTimer::Instance().TimeF();

			//移動中
			if (Target_ != pos)
			{
				//移動
				pos = Vector3F::MoveTowards(pos, Target_, CellMove * t);

				//アクターの座標を設定
				actor->Position(pos);
			}
		}

		/**
		 * @brief		移動
		 * @param[in]	mx		横移動量
		 * @param[in]	my		縦移動量
		 * @param[in]	mz		奥移動量
		 * @return		true	成功
		 *				false	失敗
		 */
		bool Push(const int mx, const int my, const int mz)
		{
			//移動先が壁
			if (Stage_->IsWall(CellPosition_->Position().x + mx, CellPosition_->Position().y + my))
			{
				return false;
			}
			//移動先がオブジェクト
			if(Stage_->GetObject(CellPosition_->Position().x + mx, CellPosition_->Position().y + my))
			{
				return false;
			}
			//移動成功
			CellPosition_->Move(mx, my, 0);
			Target_ = CellPosition_->ScreenPosition();
			return true;
		}

		/**
		 * @brief		セル座標
		 */
		const Vector3I& Position() const {
			return CellPosition_->Position();
		}
	};
	//ポインタ置き換え
	using PushMovePtr = std::shared_ptr<PushMove>;

}