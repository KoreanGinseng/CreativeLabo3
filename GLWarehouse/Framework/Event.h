#pragma once

#include	"Actor.h"

namespace Sample {

	/**
	 * @brief		マウスオーバーイベント
	 */
	class IMouseOver {
	private:
		bool		bOver = false;
	public:
		virtual void SetMouseOver(bool bc) {
			if (bOver && !bc)
			{
				OnMouseOut();
			}
			else if (!bOver && bc)
			{
				OnMouseIn();
			}
			bOver = bc;
		}
		virtual void OnMouseIn() = 0;
		virtual void OnMouseOut() = 0;
	};
	//ポインタ置き換え
	using MouseOverPtr = std::shared_ptr<IMouseOver>;
	//動的配列置き換え
	using MouseOverArray = std::vector<MouseOverPtr>;

	/**
	 * @brief		クリックイベント
	 */
	class __declspec(novtable) IClickable {
	public:
		virtual void OnClickEvent() = 0;
	};

	//ポインタ置き換え
	using ClickablePtr = std::shared_ptr<IClickable>;
	//動的配列置き換え
	using ClickableArray = std::vector<ClickablePtr>;

	/**
	 * @brief		コリジョンイベント
	 */
	class ICollisionEvent {
	private:
		ActorList	CollisionActorList_;
	public:
		void OnCollisionEvent(ActorPtr& other, bool col) {
			auto obj = std::find(CollisionActorList_.begin(), CollisionActorList_.end(), other);
			if (col && obj != CollisionActorList_.end())
			{
				OnCollisionStay(other);
			}
			else if (col && obj == CollisionActorList_.end())
			{
				OnCollisionEnter(other);
				CollisionActorList_.push_back(other);
			}
			else if (!col && obj != CollisionActorList_.end())
			{
				OnCollisionExit(other);
				CollisionActorList_.erase(obj);
			}
		}
		virtual void OnCollisionEnter(ActorPtr& other) = 0;
		virtual void OnCollisionStay(ActorPtr& other) = 0;
		virtual void OnCollisionExit(ActorPtr& other) = 0;
	};

	//ポインタ置き換え
	using CollisionEventPtr = std::shared_ptr<ICollisionEvent>;
	//動的配列置き換え
	using CollisionEventArray = std::vector<CollisionEventPtr>;

}