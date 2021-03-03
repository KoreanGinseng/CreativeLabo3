#pragma once

#include	"Actor.h"
#include	"JsonUtillity.h"

namespace Sample {

	/**
	 * @brief		Json変換用インターフェイス
	 */
	class __declspec(novtable) IComponentParser {
	public:
		virtual bool ToJson(ComponentPtr pt, const std::string& key, nlohmann::json& os) = 0;
		virtual ComponentPtr Create(nlohmann::json& os) = 0;
		virtual void Create(Actor* act, nlohmann::json& os) = 0;
		virtual bool IsParser(ComponentPtr pt) = 0;
	};
	//ポインタ置き換え
	using ComponentParserPtr = std::shared_ptr<IComponentParser>;


	template<typename Obj>
	class ComponentParser : public IComponentParser {
	private:
		class __declspec(novtable) IValueConnect {
		public:
			virtual void ToJson(Obj& o, nlohmann::json& os) = 0;
			virtual void FromJson(Obj& o, nlohmann::json& os) = 0;
		};

		template < typename T >
		class ValueConnect : public IValueConnect {
		private:
			using Ptr = T Obj::*;
			Ptr				Data_;
			std::string		Name_;
		public:
			ValueConnect(Ptr p, std::string n)
			: Data_(p)
			, Name_(n)
			{
			}

			void ToJson(Obj& o, nlohmann::json& os) override
			{
				JsonUtillity<T>::ToJson(o.*Data_, os[Name_]);
			}

			void FromJson(Obj& o, nlohmann::json& os) override
			{
				JsonUtillity<T>::FromJson(o.*Data_, os[Name_]);
			}
		};

		using ValueConnectPtr = std::shared_ptr <IValueConnect>;
		using ValueConnectList = std::vector<ValueConnectPtr>;
		ValueConnectList		ValueList_;
	public:

		bool ToJson(ComponentPtr pt, const std::string& key, nlohmann::json& os) override {
			auto re = std::dynamic_pointer_cast<Obj>(pt);
			if (re == nullptr)
			{
				return false;
			}
			ToJson(*re, os[key]);
			return true;
		}

		template< bool bp = HasToJson<Obj>::value >
		void ToJson(Obj& o, nlohmann::json& os)
		{
			for (auto& it : ValueList_)
			{
				it->ToJson(o, os);
			}
		}

		template<>
		void ToJson<true>(Obj& o, nlohmann::json& os)
		{
			o.ToJson(os);
		}

		template< bool bp = HasFromJson<Obj>::value >
		void FromJson(std::shared_ptr<Obj> pt, nlohmann::json& os)
		{
			for (auto& it : ValueList_)
			{
				it->FromJson(*pt, os);
			}
		}

		template<>
		void FromJson<true>(std::shared_ptr<Obj> pt, nlohmann::json & os)
		{
			pt->FromJson(os);
		}

		ComponentPtr Create(nlohmann::json& os) override {
			std::shared_ptr<Obj> obj = std::make_shared<Obj>();
			FromJson(obj, os);
			return obj;
		}

		template<int n = (std::is_base_of_v<IParameter, Obj >) | (std::is_base_of_v<IAction, Obj > << 1) | (std::is_base_of_v<IRender, Obj > << 2) >
		void CreateImpl(Actor* act, nlohmann::json& os)
		{
		}

		template<>
		void CreateImpl<0x01>(Actor* act, nlohmann::json& os)
		{
			auto obj = act->AddParameter<Obj>();
			FromJson(obj, os);
		}

		template<>
		void CreateImpl<0x02>(Actor* act, nlohmann::json& os)
		{
			auto obj = act->AddAction<Obj>();
			FromJson(obj, os);
		}

		template<>
		void CreateImpl<0x04>(Actor* act, nlohmann::json& os)
		{
			auto obj = act->AddRender<Obj>();
			FromJson(obj, os);
		}

		void Create(Actor* act, nlohmann::json& os) override {
			CreateImpl(act, os);
		}

		bool IsParser(ComponentPtr pt) override {
			return std::dynamic_pointer_cast<Obj>(pt) != nullptr;
		}

		template <typename T>
		void Register(T Obj::* p, std::string n)
		{
			auto v = std::make_shared<ValueConnect<T> >(p, n);
			ValueList_.push_back(v);
		}
	};

	/**
	 * @brief		登録管理用クラス
	 */
	class ParserManager {
	private:
		/**
		 * @brief		アクター生成インターフェイス
		 */
		class __declspec(novtable) IActorCreator {
		public:
			virtual ActorPtr Create(const std::string& key) const = 0;
		};
		/**
		 * @brief		アクター生成
		 */
		template< typename T >
		class ActorCreator : public IActorCreator {
		public:
			ActorPtr Create(const std::string& key) const override {
				return std::make_shared<T>(key.c_str());
			}
		};
		using ActorCreatorPtr = std::shared_ptr <IActorCreator>;
		/** 登録アクター */
		std::unordered_map<std::string, ActorCreatorPtr >		Actors_;
		/** 登録コンポーネント */
		std::unordered_map<std::string, ComponentParserPtr>		Components_;

		ParserManager()
		: Actors_()
		, Components_() {
		}
		ParserManager(const ParserManager& obj) = delete;
		ParserManager& operator=(const ParserManager&) = delete;
		virtual ~ParserManager() = default;
	public:
		/**
		 * @brief		アクター生成の登録
		 * @param[in]	key		登録キー
		 */
		template< typename T >
		void AddActorType(const std::string& key) {
			Actors_[key] = std::make_shared< ActorCreator< T > >();
		}

		/**
		 * @brief		アクター生成
		 * @param[in]	key		登録キー
		 */
		ActorPtr CreateActor(const std::string& key) {
			auto it = Actors_.find(key);
			if (it == Actors_.end())
			{
				return ActorPtr();
			}
			return it->second->Create(key);
		}

		/**
		 * @brief		コンポーネントからJsonデータ生成
		 * @param[in]	pt		対象とするコンポ―ネント
		 * @param[in]	os		ＪＳＯＮデータ
		 */
		void ToJson(ComponentPtr pt, nlohmann::json& os) {
			for (auto it = Components_.begin(); it != Components_.end(); ++it)
			{
				if(it->second->ToJson(pt, it->first, os))
				{
					return;
				}
			}
		}

		/**
		 * @brief		Jsonデータからのコンポーネント生成
		 * @param[in]	os		ＪＳＯＮデータ
		 * @return		生成されたコンポーネント
		 */
		ComponentPtr Create(nlohmann::json& os) {
			for (auto it = os.begin(); it != os.end(); ++it)
			{
				auto f = Components_.find(it.key());
				if (f == Components_.end())
				{
					continue;
				}
				return f->second->Create(os[it.key()]);
			}
		}

		/**
		 * @brief		Jsonデータからのコンポーネント生成
		 * @param[in]	act		取り付けるアクター
		 * @param[in]	os		ＪＳＯＮデータ
		 */
		void Create(Actor* act, nlohmann::json& os) {
			for (auto it = os.begin(); it != os.end(); ++it)
			{
				auto f = Components_.find(it.key());
				if (f == Components_.end())
				{
					continue;
				}
				f->second->Create(act, os[it.key()]);
			}
		}

		/**
		 * @brief		コンポーネント解析生成の登録
		 * @param[in]	key		登録キー
		 * @return		生成されたパーサー
		 */
		template <typename T>
		std::shared_ptr< ComponentParser<T> > Register(const std::string& key)
		{
			auto v = std::make_shared<ComponentParser<T> >();
			Components_[key] = v;
			return v;
		}

		static ParserManager& Instance() {
			static ParserManager t;
			return t;
		}
	};

	//登録支援用マクロ
#define		REGISTER_COMPONENT(com)			\
	static void RegisterComponent() {		\
		using ThisType = com;				\
		auto pars = ParserManager::Instance().Register<com>(#com);

#define		REGISTER_PARAMETER(p)			pars->Register(&ThisType::p, #p);

#define		REGISTER_END				}

#define		REGISTER_COMPONENTONLY(com)									\
	static void RegisterComponent() {									\
		using ThisType = com;											\
		auto pars = ParserManager::Instance().Register<com>(#com);		\
	}
}