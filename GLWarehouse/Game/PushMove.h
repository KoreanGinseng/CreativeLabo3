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
		 * @brief		�R���X�g���N�^
		 */
		PushMove()
		: ActionBase() 
		, Target_()
		, CellPosition_()
		, Stage_() {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		PushMove(const PushMove& obj)
		: ActionBase(obj) 
		, Target_()
		, CellPosition_()
		, Stage_() {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		~PushMove() {
		}

		/**
		 * @brief		�J�n
		 */
		void Start() override {
			CellPosition_ = actor->GetParameter<CellPosition>();
			Target_ = CellPosition_->ScreenPosition();
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
		}

		/**
		 * @brief		�ړ�
		 * @param[in]	mx		���ړ���
		 * @param[in]	my		�c�ړ���
		 * @param[in]	mz		���ړ���
		 * @return		true	����
		 *				false	���s
		 */
		bool Push(const int mx, const int my, const int mz)
		{
			//�ړ��悪��
			if (Stage_->IsWall(CellPosition_->Position().x + mx, CellPosition_->Position().y + my))
			{
				return false;
			}
			//�ړ��悪�I�u�W�F�N�g
			if(Stage_->GetObject(CellPosition_->Position().x + mx, CellPosition_->Position().y + my))
			{
				return false;
			}
			//�ړ�����
			CellPosition_->Move(mx, my, 0);
			Target_ = CellPosition_->ScreenPosition();
			return true;
		}

		/**
		 * @brief		�Z�����W
		 */
		const Vector3I& Position() const {
			return CellPosition_->Position();
		}
	};
	//�|�C���^�u������
	using PushMovePtr = std::shared_ptr<PushMove>;

}