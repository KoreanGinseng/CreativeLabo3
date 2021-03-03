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
		 * @brief		�R���X�g���N�^
		 */
		KeyMove()
		: ActionBase() 
		, Target_()
		, CellPosition_()
		, Animator_() 
		, Stage_() {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		KeyMove(const KeyMove& obj)
		: ActionBase(obj) 
		, Target_()
		, CellPosition_()
		, Animator_() 
		, Stage_() {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		~KeyMove() {
		}

		/**
		 * @brief		�J�n
		 */
		void Start() override {
			CellPosition_ = actor->GetParameter<CellPosition>();
			Target_ = CellPosition_->ScreenPosition();
			Animator_ = actor->GetAction<SpriteAnimator>();
			Stage_ = SceneManager::Instance().CurrentScene()->FindParameter<Stage>();
		}

		/**
		 * @brief		�X�V
		 */
		void Update() override {
			//�A�N�^�[�̍��W���擾
			Vector3F pos = actor->Position();

			//�o�ߎ��� ��Unity��Time.deltaTime�ɑΉ�
			float t = FrameTimer::Instance().TimeF();

			//�ړ���
			if (Target_ != pos)
			{
				//�ړ�
				pos = Vector3F::MoveTowards(pos, Target_, CellMove * t);

				//�A�N�^�[�̍��W��ݒ�
				actor->Position(pos);
			}
			else
			{
				//�ړ�����
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
		 * @brief		�ړ�
		 * @param[in]	mx		���ړ���
		 * @param[in]	my		�c�ړ���
		 * @param[in]	mz		���ړ���
		 * @return		true	����
		 *				false	���s
		 */
		bool Move(const int mx, const int my, const int mz)
		{
			//�ړ��悪��
			if (Stage_->IsWall(CellPosition_->Position().x + mx, CellPosition_->Position().y + my))
			{
				return false;
			}
			//�ړ��悪�I�u�W�F�N�g
			CellPositionPtr cp = Stage_->GetObject(CellPosition_->Position().x + mx, CellPosition_->Position().y + my);
			if(cp)
			{
				PushMovePtr pt = cp->GetActor()->GetAction<PushMove>();
				if (!pt->Push(mx, my, mz))
				{
					return false;
				}
			}
			//�ړ�����
			CellPosition_->Move(mx, my, 0);
			Target_ = CellPosition_->ScreenPosition();
			return true;
		}

	};

}