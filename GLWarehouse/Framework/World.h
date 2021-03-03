#pragma once

#include	"Common.h"
#include	"Camera.h"
#include	"Collision/CollisionManager.h"

namespace Sample {

	class World
	{
	protected:
		/** ���[���h���̃A�N�^�[���X�g */
		ActorListPtr								Actors_;

		/** �J�n�����A�N�^�[���X�g */
		ActorList									StartActors_;

		/** �e�����A�N�^�[���X�g */
		ActorList									RootActors_;

		/** �X�V */
		ActorListPtr								ActionActors_;

		/** �ڐG���� */
		CollisionManager							Collisions_;

		/** �`�� */
		struct PriorityData {
			int			Priority;
			RenderArray	Renders;

			PriorityData()
			: Priority(0)
			, Renders() {}
		};
		std::vector<PriorityData>					Renders_;

		World(const World& obj) = delete;
		World& operator=(const World&) = delete;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		World()
		: Actors_() 
		, StartActors_()
		, RootActors_()
		, ActionActors_()
		, Collisions_()
		, Renders_() {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~World() {
			Release();
		}

		/**
		 * @brief		�A�N�^�[�̒ǉ�
		 * @param[in]	act		�ǉ�����A�N�^�[
		 */
		void AddActor(const ActorPtr& act)
		{
			Actors_->push_back(act);
			if (act->IsAction())
			{
				ActionActors_->push_back(act);
			}
			if (act->IsRender())
			{
				const RenderArray& ra = act->GetRenders();
				for (auto& r : ra)
				{
					auto it = std::find_if(Renders_.begin(), Renders_.end(), [&](const PriorityData& obj) { return obj.Priority == r->Priority(); });
					if (it != Renders_.end())
					{
						(*it).Renders.push_back(r);
					}
					else
					{
						PriorityData pd;
						pd.Priority = r->Priority();
						pd.Renders.push_back(r);
						Renders_.push_back(pd);
						std::sort(Renders_.begin(), Renders_.end(), [](const PriorityData& left, const PriorityData& right) { return left.Priority < right.Priority; });
					}
				}
			}
			//�R���W�����o�^
			Collisions_.Register(act);
			//�A�N�^�[�̊J�n�o�^
			StartActors_.push_back(act);
			//�e�����A�N�^�[�o�^
			if (act->Parent() == nullptr)
			{
				RootActors_.push_back(act);
			}
		}

		/**
		 * @brief		���O�ł̃A�N�^�[����
		 * @param[in]	name	���O
		 * @return		�w�肳�ꂽ���O��Actor������ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		ActorPtr Find(const char* name) const {
			auto it = std::find_if(Actors_->begin(), Actors_->end(), [&](const ActorPtr& obj) { return obj->Name() == name; });
			if (it != Actors_->end())
			{
				return *it;
			}
			return ActorPtr();
		}

		/**
		 * @brief		�A�N�^�[�̌���
		 * @return		�A�N�^�[���p����������̃N���X�̃I�u�W�F�N�g���������擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		std::shared_ptr< T > FindActor() const {
			for (auto it : *Actors_)
			{
				std::shared_ptr< T > re = std::dynamic_pointer_cast<T>(it);
				if (re)
				{
					return re;
				}
			}
			return std::shared_ptr< T >();
		}

		/**
		 * @brief		�p�����[�^�[�A�N�^�[�̌���
		 * @return		�w�肳�ꂽ�p�����[�^�[������A�N�^�[�̏ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		ActorPtr FindParameterActor() const {
			auto it = std::find_if(Actors_->begin(), Actors_->end(), [](const ActorPtr& obj) { return obj->GetParameter<T>() != nullptr; });
			if (it != Actors_->end())
			{
				return *it;
			}
			return ActorPtr();
		}

		/**
		 * @brief		�p�����[�^�[�A�N�^�[�̌���
		 * @return		�w�肳�ꂽ�p�����[�^�[������A�N�^�[��vector���擾����
		 */
		template< typename T >
		std::vector< ActorPtr > FindParameterActors() const {
			std::vector< ActorPtr > re;
			std::copy_if(Actors_->begin(), Actors_->end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetParameter<T>() != nullptr; });
			return re;
		}

		/**
		 * @brief		�p�����[�^�[�̌���
		 * @return		�w�肳�ꂽ�p�����[�^�[������ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		std::shared_ptr< T > FindParameter() const {
			auto t = FindParameterActor< T >();
			if (t)
			{
				return t->GetParameter<T>();
			}
			return std::shared_ptr< T >();
		}

		/**
		 * @brief		�p�����[�^�[�̌���
		 * @return		�w�肳�ꂽ�p�����[�^�[������ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		std::vector < std::shared_ptr< T > > FindParameters() const {
			auto t = FindParameterActors< T >();
			std::vector < std::shared_ptr< T > > re;
			std::transform(t.begin(), t.end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetParameter<T>(); });
			return re;
		}

		/**
		 * @brief		����A�N�^�[�̌���
		 * @return		�w�肳�ꂽ���삪����A�N�^�[�̏ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		ActorPtr FindActionActor() const {
			auto it = std::find_if(ActionActors_->begin(), ActionActors_->end(), [](const ActorPtr& obj) { return obj->GetAction<T>() != nullptr; });
			if (it != ActionActors_->end())
			{
				return *it;
			}
			return ActorPtr();
		}

		/**
		 * @brief		����A�N�^�[�̌���
		 * @return		�w�肳�ꂽ���삪����A�N�^�[��vector���擾����
		 */
		template< typename T >
		std::vector< ActorPtr > FindActionActors() const {
			std::vector< ActorPtr > re;
			std::copy_if(ActionActors_->begin(), ActionActors_->end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetAction<T>() != nullptr; });
			return re;
		}

		/**
		 * @brief		����̌���
		 * @return		�w�肳�ꂽ���삪����ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		std::shared_ptr< T > FindAction() const {
			auto t = FindActionActor< T >();
			if (t)
			{
				return t->GetAction<T>();
			}
			return std::shared_ptr< T >();
		}
		
		/**
		 * @brief		����̌���
		 * @return		�w�肳�ꂽ���삪����ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		std::vector < std::shared_ptr< T > > FindActions() const {
			auto t = FindActionActors< T >();
			std::vector < std::shared_ptr< T > > re;
			std::transform(t.begin(), t.end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetAction<T>(); });
			return re;
		}
		
		/**
		 * @brief		����A�N�^�[�̌���
		 * @return		�w�肳�ꂽ���삪����A�N�^�[�̏ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		ActorPtr FindRenderActor() const {
			auto it = std::find_if(Actors_->begin(), Actors_->end(), [](const ActorPtr& obj) { return obj->GetRender<T>() != nullptr; });
			if (it != Actors_->end())
			{
				return *it;
			}
			return ActorPtr();
		}

		/**
		 * @brief		����A�N�^�[�̌���
		 * @return		�w�肳�ꂽ���삪����A�N�^�[��vector���擾����
		 */
		template< typename T >
		std::vector< ActorPtr > FindRenderActors() const {
			std::vector< ActorPtr > re;
			std::copy_if(Actors_->begin(), Actors_->end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetRender<T>() != nullptr; });
			return re;
		}

		/**
		 * @brief		����̌���
		 * @return		�w�肳�ꂽ���삪����ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		std::shared_ptr< T > FindRender() const {
			auto t = FindRenderActor< T >();
			if (t)
			{
				return t->GetRender<T>();
			}
			return std::shared_ptr< T >();
		}

		/**
		 * @brief		����̌���
		 * @return		�w�肳�ꂽ���삪����ꍇ�A���̐擪���擾����
		 *				���݂��Ȃ��ꍇ�͋��shared_ptr��Ԃ�
		 */
		template< typename T >
		std::vector < std::shared_ptr< T > > FindRenders() const {
			auto t = FindRenderActors< T >();
			std::vector < std::shared_ptr< T > > re;
			std::transform(t.begin(), t.end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetRender<T>(); });
			return re;
		}

		/**
		 * @brief		���[���h�̎Q�Ƃ��Ȃ���
		 */
		virtual void Release() {
			Actors_.reset();
			Collisions_.Release();
			RootActors_.clear();
			StartActors_.clear();
		}

		ActorListPtr& Actors() { return Actors_; }
		ActorList& RootActors() { return RootActors_; }
	};

}