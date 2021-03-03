#pragma once

#include	"Common.h"
#include	"Camera.h"
#include	"Collision/CollisionManager.h"

namespace Sample {

	class World
	{
	protected:
		/** ワールド内のアクターリスト */
		ActorListPtr								Actors_;

		/** 開始準備アクターリスト */
		ActorList									StartActors_;

		/** 親無しアクターリスト */
		ActorList									RootActors_;

		/** 更新 */
		ActorListPtr								ActionActors_;

		/** 接触判定 */
		CollisionManager							Collisions_;

		/** 描画 */
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
		 * @brief		コンストラクタ
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
		 * @brief		デストラクタ
		 */
		virtual ~World() {
			Release();
		}

		/**
		 * @brief		アクターの追加
		 * @param[in]	act		追加するアクター
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
			//コリジョン登録
			Collisions_.Register(act);
			//アクターの開始登録
			StartActors_.push_back(act);
			//親無しアクター登録
			if (act->Parent() == nullptr)
			{
				RootActors_.push_back(act);
			}
		}

		/**
		 * @brief		名前でのアクター検索
		 * @param[in]	name	名前
		 * @return		指定された名前のActorがある場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		アクターの検索
		 * @return		アクターを継承した特定のクラスのオブジェクトを検索し取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		パラメーターアクターの検索
		 * @return		指定されたパラメーターがあるアクターの場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		パラメーターアクターの検索
		 * @return		指定されたパラメーターがあるアクターのvectorを取得する
		 */
		template< typename T >
		std::vector< ActorPtr > FindParameterActors() const {
			std::vector< ActorPtr > re;
			std::copy_if(Actors_->begin(), Actors_->end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetParameter<T>() != nullptr; });
			return re;
		}

		/**
		 * @brief		パラメーターの検索
		 * @return		指定されたパラメーターがある場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		パラメーターの検索
		 * @return		指定されたパラメーターがある場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		動作アクターの検索
		 * @return		指定された動作があるアクターの場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		動作アクターの検索
		 * @return		指定された動作があるアクターのvectorを取得する
		 */
		template< typename T >
		std::vector< ActorPtr > FindActionActors() const {
			std::vector< ActorPtr > re;
			std::copy_if(ActionActors_->begin(), ActionActors_->end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetAction<T>() != nullptr; });
			return re;
		}

		/**
		 * @brief		動作の検索
		 * @return		指定された動作がある場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		動作の検索
		 * @return		指定された動作がある場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		動作アクターの検索
		 * @return		指定された動作があるアクターの場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		動作アクターの検索
		 * @return		指定された動作があるアクターのvectorを取得する
		 */
		template< typename T >
		std::vector< ActorPtr > FindRenderActors() const {
			std::vector< ActorPtr > re;
			std::copy_if(Actors_->begin(), Actors_->end(), std::back_inserter(re),
				[](const ActorPtr& obj) { return obj->GetRender<T>() != nullptr; });
			return re;
		}

		/**
		 * @brief		動作の検索
		 * @return		指定された動作がある場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		動作の検索
		 * @return		指定された動作がある場合、その先頭を取得する
		 *				存在しない場合は空のshared_ptrを返す
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
		 * @brief		ワールドの参照をなくす
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