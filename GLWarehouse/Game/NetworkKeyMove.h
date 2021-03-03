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
		 * @brief		コンストラクタ
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
		 * @brief		コンストラクタ
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
		 * @brief		デストラクタ
		 */
		~NetworkKeyMove() {
		}

		/**
		 * ネットワーククライアントの設定
		 */
		void SetClient(const SampleServer::ClientPtr ct)
		{
			Client_ = ct;
		}

		/**
		 * 所有者のID設定
		 */
		void SetOwnerID(uint32_t id)
		{
			OwnerID_ = id;
		}

		/**
		 * 所有者のID取得
		 */
		uint32_t GetOwnerID() {
			return OwnerID_;
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
				//所有者が違う
				if (Client_->GetUID() != OwnerID_)
				{
					return;
				}
				//移動入力
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
		 * @brief		移動
		 * @param[in]	mx		横移動量
		 * @param[in]	my		縦移動量
		 * @return		true	成功
		 *				false	失敗
		 */
		bool Move(const int mx, const int my)
		{
			//移動入力
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
	//ポインタ置き換え
	using NetworkKeyMovePtr = std::shared_ptr<NetworkKeyMove>;

}