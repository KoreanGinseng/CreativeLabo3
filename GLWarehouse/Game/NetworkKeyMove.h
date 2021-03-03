#pragma once

#include	"PushMove.h"

namespace Sample {

	class NetworkKeyMove : public ActionBase<NetworkKeyMove> {
	private:
		SampleServer::ClientPtr Client_;
		uint32_t OwnerID_;
		Vector3F Target_;
		CellPositionPtr CellPosition_;
		SpriteAnimatorPtr Animator_;
		StagePtr Stage_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		NetworkKeyMove()
		: ActionBase() 
		, Client_()
		, OwnerID_(0)
		, Target_()
		, CellPosition_()
		, Animator_() 
		, Stage_() {
		}
		/**
		 * @brief		�R���X�g���N�^
		 */
		NetworkKeyMove(const NetworkKeyMove& obj)
		: ActionBase(obj) 
		, Client_(obj.Client_)
		, OwnerID_(obj.OwnerID_)
		, Target_()
		, CellPosition_()
		, Animator_() 
		, Stage_() {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		~NetworkKeyMove() {
		}

		/**
		 * �l�b�g���[�N�N���C�A���g�̐ݒ�
		 */
		void SetClient(const SampleServer::ClientPtr ct)
		{
			Client_ = ct;
		}

		/**
		 * ���L�҂�ID�ݒ�
		 */
		void SetOwnerID(uint32_t id)
		{
			OwnerID_ = id;
		}

		/**
		 * ���L�҂�ID�擾
		 */
		uint32_t GetOwnerID() {
			return OwnerID_;
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
				//���L�҂��Ⴄ
				if (Client_->GetUID() != OwnerID_)
				{
					return;
				}
				//�ړ�����
				if (InputManager::Instance().IsPress("Down"))
				{
					int mv[]{ 0,1 };
					Client_->Send(mv, sizeof(mv), 0, SampleServer::SendTarget::RoomClient, 0, SampleServer::MessageType::Other, SampleServer::SendType::RUdp);
				}
				else if (InputManager::Instance().IsPress("Right"))
				{
					int mv[]{ 1,0 };
					Client_->Send(mv, sizeof(mv), 0, SampleServer::SendTarget::RoomClient, 0, SampleServer::MessageType::Other, SampleServer::SendType::RUdp);
				}
				else if (InputManager::Instance().IsPress("Left"))
				{
					int mv[]{ -1,0 };
					Client_->Send(mv, sizeof(mv), 0, SampleServer::SendTarget::RoomClient, 0, SampleServer::MessageType::Other, SampleServer::SendType::RUdp);
				}
				else if (InputManager::Instance().IsPress("Up"))
				{
					int mv[]{ 0,-1 };
					Client_->Send(mv, sizeof(mv), 0, SampleServer::SendTarget::RoomClient, 0, SampleServer::MessageType::Other, SampleServer::SendType::RUdp);
				}
			}
		}

		/**
		 * @brief		�ړ�
		 * @param[in]	mx		���ړ���
		 * @param[in]	my		�c�ړ���
		 * @return		true	����
		 *				false	���s
		 */
		bool Move(const int mx, const int my)
		{
			//�ړ�����
			if (my == 1)
			{
				Animator_->ChangeAnimation("Down");
				Move(0, 1, 0);
			}
			else if (mx == 1)
			{
				Animator_->ChangeAnimation("Right");
				Move(1, 0, 0);
			}
			else if (mx == -1)
			{
				Animator_->ChangeAnimation("Left");
				Move(-1, 0, 0);
			}
			else if (my == -1)
			{
				Animator_->ChangeAnimation("Up");
				Move(0, -1, 0);
			}
			return true;
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
	//�|�C���^�u������
	using NetworkKeyMovePtr = std::shared_ptr<NetworkKeyMove>;

}