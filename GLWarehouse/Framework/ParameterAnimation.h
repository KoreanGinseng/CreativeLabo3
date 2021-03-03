#pragma once

#include	"KeyFrameAnimation.h"
#include	"ParserManager.h"

namespace Sample {

	/** メソッド存在確認 */
#define		ON_GETSET(Name)														\
	HAS_FUNCNAME(Name)															\
	template<typename Obj, typename Type> class OnGetSet##Name {				\
	public:																		\
		/** ゲッター */															\
		using OnGetFunction = std::function< Type(Obj&) >;						\
		/** セッター */															\
		using OnSetFunction = std::function< void(Obj&, Type) >;				\
		OnGetSet##Name(OnGetFunction& gf, OnSetFunction& sf){					\
			gf = Get();			sf = Set();										\
		}																		\
		/** Ｇｅｔ */															\
		template < bool bp = Has##Name<Obj>::value>								\
		OnGetFunction Get()														\
		{																		\
			return OnGetFunction();												\
		}																		\
		template <  >															\
		OnGetFunction Get<true>()												\
		{																		\
			return OnGetFunction((const Type(Obj::*)() const)&Obj::Name);		\
		}																		\
		/** Ｓｅｔ */															\
		template < bool bp = Has##Name<Obj>::value>								\
		OnSetFunction Set()														\
		{																		\
			return OnSetFunction();												\
		}																		\
		template <  >															\
		OnSetFunction Set<true>()												\
		{																		\
			return OnSetFunction((void(Obj::*)(Type))&Obj::Name);				\
		}																		\
	};

	//Get,Setの登録
	ON_GETSET(X);
	ON_GETSET(Y);
	ON_GETSET(Z);
	ON_GETSET(Alpha);

	template<typename Obj, typename Type>
	class ParameterAnimation : public ActionBase< ParameterAnimation<Obj, Type> > {
	private:
		/** 基底クラス */
		using Super = ActionBase< ParameterAnimation<Obj, Type> >;
		/** ゲッター */
		using OnGetFunction = std::function< Type(Obj&) >;
		/** セッター */
		using OnSetFunction = std::function< void(Obj&, Type) >;
		/** アニメーションデータ */
		using AnimationData = typename KeyFrameAnimation<Type>::Animation;

		/** パラメーター名 */
		std::string					ParamName_;

		/** ゲット */
		OnGetFunction				OnGet;
		/** セット */
		OnSetFunction				OnSet;

		/** オブジェクト */
		Obj*						RawObject_;
		std::shared_ptr< Obj >		Object_;

		/** 値 */
		Type						Value_;
		/** 現在のアニメーション時間 */
		float						NowTime_;

		/** アニメーション */
		std::shared_ptr< KeyFrameAnimation< Type > >	Animation_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		ParameterAnimation()
		: Super()
		, ParamName_()
		, OnGet(nullptr)
		, OnSet(nullptr)
		, RawObject_(nullptr)
		, Object_()
		, Value_()
		, NowTime_(0.0f)
		, Animation_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		ParameterAnimation(const char* ParamName)
		: Super()
		, ParamName_(ParamName)
		, OnGet(nullptr)
		, OnSet(nullptr)
		, RawObject_(nullptr)
		, Object_()
		, Value_()
		, NowTime_(0.0f)
		, Animation_() {
			SetFunction();
		}
		/**
		 * @brief		コンストラクタ
		 */
		ParameterAnimation(OnGetFunction get, OnSetFunction set,
			const AnimationData& anim, std::shared_ptr< InterpolationValue<Type> > ip)
		: Super()
		, ParamName_()
		, OnGet(get)
		, OnSet(set)
		, RawObject_(nullptr)
		, Object_()
		, Value_()
		, NowTime_(0.0f)
		, Animation_(std::make_shared<KeyFrameAnimation< Type >>(anim, ip)) {
		}
		
		/**
		 * @brief		コンストラクタ
		 */
		ParameterAnimation(const ParameterAnimation& obj)
		: Super(obj)
		, ParamName_(obj.ParamName_)
		, OnGet(obj.OnGet)
		, OnSet(obj.OnSet)
		, RawObject_(obj.RawObject_)
		, Object_(obj.Object_)
		, Value_(obj.Value_)
		, NowTime_(obj.NowTime_)
		, Animation_(obj.Animation_) {
		}

		/**
		 * @brief		初期化
		 */
		void Initialize()
		{
		}


		/**
		 * @brief		初期化
		 */
		void Initialize(const std::string& name)
		{
			ParamName_ = name;
			SetFunction();
		}

		/**
		 * @brief		初期化
		 */
		void Initialize(OnGetFunction get, OnSetFunction set)
		{
			OnGet = get;
			OnSet = set;
		}

		/**
		 * @brief		アニメーション登録
		 * @param[in]	anim	アニメーション
		 * @param[in]	ip		補間
		 */
		void Register(const AnimationData& anim, std::shared_ptr< InterpolationValue<Type> > ip) {
			Animation_ = std::make_shared<KeyFrameAnimation< Type >>(anim, ip);
		}

		/**
		 * @brief		開始
		 */
		void Start() override {
			RawObject_ = dynamic_cast<Obj*>(this->actor);
			if (RawObject_)
			{
				if (OnGet)
				{
					Value_ = OnGet(*RawObject_);
				}
			}
			else
			{
				Object_ = this->actor->GetComponent<Obj>();
				if (OnGet)
				{
					Value_ = OnGet(*Object_);
				}
			}
		}

		/**
		 * @brief		更新
		 */
		void Update() override {
			//時間経過
			NowTime_ += FrameTimer::Instance().TimeF();
			//値の設定
			Value_ = Animation_->GetValue(NowTime_);
			if (OnGet == nullptr || OnSet == nullptr)
			{
				return;
			}

			if (RawObject_)
			{
				OnSet(*RawObject_, Value_);
			}
			else
			{
				OnSet(*Object_, Value_);
			}
		}

		/**
		 * @brief		独自のＪＳＯＮ出力実装
		 */
		void ToJson(nlohmann::json& os)
		{
			os["Parameter"] = ParamName_;
			Animation_->ToJson(os["Animation"]);
		}
		/**
		 * @brief		独自のＪＳＯＮ読み込み実装
		 */
		void FromJson(nlohmann::json& os)
		{
			os["Parameter"].get_to(ParamName_);
			Animation_ = std::make_shared<KeyFrameAnimation<Type>>();
			Animation_->FromJson(os["Animation"]);
			SetFunction();
		}

		/**
		 * @brief		関数を設定
		 */
		void SetFunction() {
			if (ParamName_ == "X") { OnGetSetX<Obj, Type> func(OnGet, OnSet); }
			else if (ParamName_ == "Y"){ OnGetSetY<Obj, Type> func(OnGet, OnSet); }
			else if (ParamName_ == "Z"){ OnGetSetZ<Obj, Type> func(OnGet, OnSet); }
			else if (ParamName_ == "Alpha") { OnGetSetAlpha<Obj, Type> func(OnGet, OnSet); }
		}
	};
}