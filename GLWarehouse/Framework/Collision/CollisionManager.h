#pragma once

#include	"Collider.h"
#include	"../Event.h"
#include	"../InputManager.h"

namespace Sample {
	
	/**
	 * @brief			�`�󂲂Ƃ̐ڐG�֐�
	 */
	template <typename T1, typename T2>
	class CollisionFunction {
	public:
		bool Collision(const T1* pt1, const T2* pt2) const {
			return false;
		}
	};
	/**
	 * @brief			��`���m�̔���
	 */
	template <>
	class CollisionFunction<ColliderRect, ColliderRect>{
	public:
		bool Collision(const ColliderRect* pt1, const ColliderRect* pt2) const {
			return pt1->Rectangle().Collision(pt2->Rectangle());
		}
	};
	/**
	 * @brief			�~���m�̔���
	 */
	template <>
	class CollisionFunction<ColliderCircle, ColliderCircle> {
	public:
		bool Collision(const ColliderCircle* pt1, const ColliderCircle* pt2) const {
			return pt1->Circle().Collision(pt2->Circle());
		}
	};
	/**
	 * @brief			��`�Ɖ~�̔���
	 */
	template <>
	class CollisionFunction<ColliderRect, ColliderCircle> {
	public:
		bool Collision(const ColliderRect* pt1, const ColliderCircle* pt2) const {
			//��`����K�v�ȏ������߂�
			RectangleF r = pt1->Rectangle();
			float cx, cy;
			r.Center(cx, cy);
			float rw = r.Width() * 0.5f;
			float rh = r.Height() * 0.5f;
			//����̉~
			CircleF cir = pt2->Circle();
			//���S�͈͓�
			if (cir.X >= cx - rw - cir.Radius && cir.X <= cx + rw + cir.Radius && cir.Y >= cy - rh && cir.Y <= cy + rh)
			{
				return true;
			}
			else if (cir.X >= cx - rw && cir.X <= cx + rw && cir.Y >= cy - rh - cir.Radius && cir.Y <= cy + rh + cir.Radius)
			{
				return true;
			}
			//�p�Ƃ̔���
			float sx = cir.X - ((cir.X < cx) ? cx - rw : cx + rw);
			float sy = cir.Y - ((cir.Y < cy) ? cy - rh : cy + rh);
			float d = (sx * sx) + (sy * sy);
			return d < (cir.Radius * cir.Radius);
		}
	};

	class CollisionManager {
	public:
		/**
		 * @brief		�ڐG����ƃC�x���g�̃Z�b�g
		 */
		struct CollisionSet
		{
			ActorPtr				Actor;
			ColliderPtr				Collider;
			MouseOverArray			OverEvents;
			ClickableArray			ClickEvents;
			CollisionEventArray		CollisionEvents;
		};
		//���I�z��u������
		using CollisionSetList = std::list<CollisionSet>;
		using CollisionSetListPtr = std::shared_ptr<CollisionSetList>;
		using CollisionSetListArray = std::vector< CollisionSetListPtr >;
		using CollisionSetListArrayPtr = std::shared_ptr<CollisionSetListArray>;
		using CollisionLayerPair = std::pair< CollisionSetListPtr, CollisionSetListPtr>;
		using CollisionLayerPairArray = std::vector< CollisionLayerPair >;
		using CollisionLayerPairArrayPtr = std::shared_ptr<CollisionLayerPairArray>;

		/** �ڐG���X�g */
		CollisionSetListPtr			CollisionSet_;
		/** �ڐG���X�g */
		CollisionSetListPtr			CollisionSetMouse_;
		/** �ڐG���X�g(���C���[�P��) */
		CollisionSetListArrayPtr	CollisionSetLayers_;
		/** �ڐG���C���[�y�A */
		CollisionLayerPairArrayPtr	CollisionLayerPair_;

		/**
		 * @brief		������
		 */
		void Initialize(std::vector< uint32_t > matrix) {
			CollisionSet_ = std::make_shared< CollisionSetList >();
			CollisionSetMouse_ = std::make_shared< CollisionSetList >();
			CollisionSetLayers_ = std::make_shared< CollisionSetListArray >();
			CollisionLayerPair_ = std::make_shared< CollisionLayerPairArray >();
			CollisionSetLayers_->resize(matrix.size());
			for (size_t i = 0; i < matrix.size(); i++)
			{
				CollisionSetLayers_->at(i) = std::make_shared<CollisionSetList>();
			}
			for (size_t i = 0; i < matrix.size(); i++)
			{
				for (size_t j = i; j < matrix.size(); j++)
				{
					if (matrix[i] & (1 << j))
					{
						CollisionLayerPair_->push_back(std::make_pair(CollisionSetLayers_->at(i), CollisionSetLayers_->at(j)));
					}
				}
			}
		}

		/**
		 * @brief		���
		 */
		void Release() {
			CollisionSet_.reset();
			CollisionSetMouse_.reset();
			CollisionSetLayers_.reset();
			CollisionLayerPair_.reset();
		}

		/**
		 * @brief		�o�^
		 * @param[in]	act	�A�N�^�[
		 */
		void Register(ActorPtr act)
		{
			//�C�x���g�������Ɣ���s�v
			auto overs = act->GetComponents<IMouseOver>();
			auto clicks = act->GetComponents<IClickable>();
			auto colevs = act->GetComponents<ICollisionEvent>();
			//�ڐG����o�^
			auto cols = act->GetComponents<ICollider>();
			for (auto v : cols)
			{
				CollisionSet set;
				set.Actor = act;
				set.Collider = v;
				set.OverEvents = overs;
				set.ClickEvents = clicks;
				set.CollisionEvents = colevs;
				CollisionSet_->push_back(set);

				//�}�E�X�C�x���g�o�^
				if (clicks.size() > 0 || overs.size() > 0)
				{
					CollisionSetMouse_->push_back(set);
				}
				//���C���[���Ƃ̓o�^
				int size = CollisionSetLayers_->size();
				if (size > v->Layer())
				{
					CollisionSetListPtr& layerset = CollisionSetLayers_->at(v->Layer());
					if (!layerset)
					{
						layerset = std::make_shared<CollisionSetList>();
					}
					layerset->push_back(set);
				}
			}
		}

		/**
		 * @brief		�R���W�����C�x���g���s
		 */
		void Collision()
		{
			//�N���b�N����p�}�E�X���W
			float x = static_cast<float>(InputManager::Instance().GetMouseX());
			float y = static_cast<float>(InputManager::Instance().GetMouseY());
			bool Enter = InputManager::Instance().IsPush("Enter");
			//�o�^�R���C�_�Ƃ̐ڐG����
			for (auto& v : *CollisionSetMouse_)
			{
				//�ڐG����
				bool bcol = v.Collider->OnCollisionPoint(x, y);
				//�}�E�X�I�[�o�[�C�x���g�̔��s
				for (auto ev : v.OverEvents)
				{
					ev->SetMouseOver(bcol);
				}
				//�ڐG�������s
				if (bcol)
				{
					if (Enter)
					{
						//�o�^���ꂽ�S�ẴN���b�N�C�x���g���s
						for (auto ev : v.ClickEvents)
						{
							ev->OnClickEvent();
						}
						return;
					}
				}
			}
			//�y�A�Ŕ���
			for (auto& it : *CollisionLayerPair_)
			{
				Collision(it.first, it.second);
			}
			//�o�^�폜
			CollisionSet_->erase(std::remove_if(CollisionSet_->begin(), CollisionSet_->end(),
				[](const CollisionSet& act) { return act.Actor->IsEnd(); }), CollisionSet_->end());
			CollisionSetMouse_->erase(std::remove_if(CollisionSetMouse_->begin(), CollisionSetMouse_->end(),
				[](const CollisionSet& act) { return act.Actor->IsEnd(); }), CollisionSetMouse_->end());
			for(auto& it : *CollisionSetLayers_)
			{
				it->erase(std::remove_if(it->begin(), it->end(),
					[](const CollisionSet& act) { return act.Actor->IsEnd(); }), it->end());
			}
		}

		/**
		 * @brief		�R���W�����C�x���g���s
		 */
		void Collision(CollisionSetListPtr& pt1, CollisionSetListPtr& pt2)
		{
			//���ꃊ�X�g�Ƃ̔���
			if (pt1 == pt2)
			{
				for (auto it1 = pt1->begin(); it1 != pt1->end(); ++it1)
				{
					auto it2 = it1;
					++it2;
					for (; it2 != pt1->end(); ++it2)
					{
						Collision(*it1, *it2);
					}
				}
				return;
			}

			//�ʃ��X�g�ł̔���
			for (auto& it1 : *pt1)
			{
				for (auto& it2 : *pt2)
				{
					Collision(it1, it2);
				}
			}
		}

		/**
		 * @brief		�R���W�����C�x���g���s
		 */
		void Collision(CollisionSet& pt1, CollisionSet& pt2)
		{
			//�ڐG����
			bool col = false;
			//��`���m�̔���
			if (pt1.Collider->Type() == ColliderType::Rectangle &&
				pt2.Collider->Type() == ColliderType::Rectangle)
			{
				CollisionFunction<ColliderRect, ColliderRect> func;
				col = func.Collision((ColliderRect*)pt1.Collider.get(), (ColliderRect*)pt2.Collider.get());
			}
			//�~���m�̔���
			else if (pt1.Collider->Type() == ColliderType::Circle &&
				pt2.Collider->Type() == ColliderType::Circle)
			{
				CollisionFunction<ColliderCircle, ColliderCircle> func;
				col = func.Collision((ColliderCircle*)pt1.Collider.get(), (ColliderCircle*)pt2.Collider.get());
			}
			//��`�Ɖ~�̔���
			else if (pt1.Collider->Type() == ColliderType::Rectangle &&
				pt2.Collider->Type() == ColliderType::Circle)
			{
				CollisionFunction<ColliderRect, ColliderCircle> func;
				col = func.Collision((ColliderRect*)pt1.Collider.get(), (ColliderCircle*)pt2.Collider.get());
			}
			else if (pt1.Collider->Type() == ColliderType::Circle &&
				pt2.Collider->Type() == ColliderType::Rectangle)
			{
				CollisionFunction<ColliderRect, ColliderCircle> func;
				col = func.Collision((ColliderRect*)pt2.Collider.get(), (ColliderCircle*)pt1.Collider.get());
			}
			//�ڐG�C�x���g���s
			for (auto ev : pt1.CollisionEvents)
			{
				ev->OnCollisionEvent(pt2.Actor, col);
			}
			for (auto ev : pt2.CollisionEvents)
			{
				ev->OnCollisionEvent(pt1.Actor, col);
			}
		}
	};

}