#pragma once

#include		<Mof.h>
#include		<memory>
#include		<list>

namespace Behaviour {

	//ポインタ置き換えのための前方定義
	class BehaviourActor;
	using BehaviourActorPtr = std::shared_ptr<BehaviourActor>;
	using BehaviourActorList = std::list<BehaviourActorPtr>;

	class BehaviourActor : public std::enable_shared_from_this<BehaviourActor> {
	private:
		/** 座標 */
		CVector3			_position;

		/** ターゲット */
		BehaviourActorPtr	_target;
	public:
		bool MoveX() {
			_position.x++;
			return false;
		}

		bool MoveTarget() {
			CVector3 sub = _target->GetPosition() - _position;
			sub.Normal(sub);
			_position += sub;
			if (GetTargetDistance() <= 20)
			{
				return true;
			}
			return false;
		}

		void Render(MofU32 col) {
			CGraphicsUtilities::RenderFillCircle(_position.x, _position.y, 10, col);
		}

		void SetTarget(BehaviourActorPtr& target) { _target = target; }
		void SetPosition(const CVector3& pos) { _position = pos; }
		const float GetTargetDistance() const noexcept { return CVector3Utilities::Distance(_position, _target->GetPosition()); }
		const CVector3& GetPosition() const noexcept { return _position; }
		const float GetX() const noexcept { return _position.x; }
		const float GetY() const noexcept { return _position.y; }
		const float GetZ() const noexcept { return _position.z; }
	};

}