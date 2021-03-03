#pragma once

#include	"PushMove.h"

namespace Sample {

	class KeyMove : public ActionBase<KeyMove> {
	private:
		Vector3F Target_;
		CellPositionPtr CellPosition_;
		SpriteAnimatorPtr Animator_;
		StagePtr Stage_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		KeyMove()
		: ActionBase() 
		, Target_()
		, CellPosition_()
		, Animator_() 
		, Stage_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		KeyMove(const KeyMove& obj)
		: ActionBase(obj) 
		, Target_()
		, CellPosition_()
		, Animator_() 
		, Stage_() {
		}

		/**
		 * @brief		デストラクタ
		 */
		~KeyMove() {
		}

		/**
		 * @brief		開始
		 */
		void Start() override {
			CellPosition_ = actor->GetParameter<CellPosition>();
			Target_ = CellPosition_->ScreenPosition();
			Animator_ = actor->GetAction<SpriteAnimator>();
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
			else
			{
				//移動入力
				if (InputManager::Instance().IsPress("Down"))
				{
					Animator_->ChangeAnimation("Down");
					Move(0, 1, 0);
				}
				else if (InputManager::Instance().IsPress("Right"))
				{
					Animator_->ChangeAnimation("Right");
					Move(1, 0, 0);
				}
				else if (InputManager::Instance().IsPress("Left"))
				{
					Animator_->ChangeAnimation("Left");
					Move(-1, 0, 0);
				}
				else if (InputManager::Instance().IsPress("Up"))
				{
					Animator_->ChangeAnimation("Up");
					Move(0, -1, 0);
				}
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
		bool Move(const int mx, const int my, const int mz)
		{
			//移動先が壁
			if (Stage_->IsWall(CellPosition_->Position().x + mx, CellPosition_->Position().y + my))
			{
				return false;
			}
			//移動先がオブジェクト
			CellPositionPtr cp = Stage_->GetObject(CellPosition_->Position().x + mx, CellPosition_->Position().y + my);
			if(cp)
			{
				PushMovePtr pt = cp->GetActor()->GetAction<PushMove>();
				if (!pt->Push(mx, my, mz))
				{
					return false;
				}
			}
			//移動成功
			CellPosition_->Move(mx, my, 0);
			Target_ = CellPosition_->ScreenPosition();
			return true;
		}

	};

}