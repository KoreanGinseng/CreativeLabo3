#pragma once

#include	"KeyFrameAnimation.h"
#include	"ParserManager.h"

namespace Sample {

	/** ���\�b�h���݊m�F */
#define		ON_GETSET(Name)														\
	HAS_FUNCNAME(Name)															\
	template<typename Obj, typename Type> class OnGetSet##Name {				\
	public:																		\
		/** �Q�b�^�[ */															\
		using OnGetFunction = std::function< Type(Obj&) >;						\
		/** �Z�b�^�[ */															\
		using OnSetFunction = std::function< void(Obj&, Type) >;				\
		OnGetSet##Name(OnGetFunction& gf, OnSetFunction& sf){					\
			gf = Get();			sf = Set();										\
		}																		\
		/** �f���� */															\
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
		/** �r���� */															\
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

	//Get,Set�̓o�^
	ON_GETSET(X);
	ON_GETSET(Y);
	ON_GETSET(Z);
	ON_GETSET(Alpha);

	template<typename Obj, typename Type>
	class ParameterAnimation : public ActionBase< ParameterAnimation<Obj, Type> > {
	private:
		/** ���N���X */
		using Super = ActionBase< ParameterAnimation<Obj, Type> >;
		/** �Q�b�^�[ */
		using OnGetFunction = std::function< Type(Obj&) >;
		/** �Z�b�^�[ */
		using OnSetFunction = std::function< void(Obj&, Type) >;
		/** �A�j���[�V�����f�[�^ */
		using AnimationData = typename KeyFrameAnimation<Type>::Animation;

		/** �p�����[�^�[�� */
		std::string					ParamName_;

		/** �Q�b�g */
		OnGetFunction				OnGet;
		/** �Z�b�g */
		OnSetFunction				OnSet;

		/** �I�u�W�F�N�g */
		Obj*						RawObject_;
		std::shared_ptr< Obj >		Object_;

		/** �l */
		Type						Value_;
		/** ���݂̃A�j���[�V�������� */
		float						NowTime_;

		/** �A�j���[�V���� */
		std::shared_ptr< KeyFrameAnimation< Type > >	Animation_;
	public:
		/**
		 * @brief		�R���X�g���N�^
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
		 * @brief		�R���X�g���N�^
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
		 * @brief		�R���X�g���N�^
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
		 * @brief		�R���X�g���N�^
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
		 * @brief		������
		 */
		void Initialize()
		{
		}


		/**
		 * @brief		������
		 */
		void Initialize(const std::string& name)
		{
			ParamName_ = name;
			SetFunction();
		}

		/**
		 * @brief		������
		 */
		void Initialize(OnGetFunction get, OnSetFunction set)
		{
			OnGet = get;
			OnSet = set;
		}

		/**
		 * @brief		�A�j���[�V�����o�^
		 * @param[in]	anim	�A�j���[�V����
		 * @param[in]	ip		���
		 */
		void Register(const AnimationData& anim, std::shared_ptr< InterpolationValue<Type> > ip) {
			Animation_ = std::make_shared<KeyFrameAnimation< Type >>(anim, ip);
		}

		/**
		 * @brief		�J�n
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
		 * @brief		�X�V
		 */
		void Update() override {
			//���Ԍo��
			NowTime_ += FrameTimer::Instance().TimeF();
			//�l�̐ݒ�
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
		 * @brief		�Ǝ��̂i�r�n�m�o�͎���
		 */
		void ToJson(nlohmann::json& os)
		{
			os["Parameter"] = ParamName_;
			Animation_->ToJson(os["Animation"]);
		}
		/**
		 * @brief		�Ǝ��̂i�r�n�m�ǂݍ��ݎ���
		 */
		void FromJson(nlohmann::json& os)
		{
			os["Parameter"].get_to(ParamName_);
			Animation_ = std::make_shared<KeyFrameAnimation<Type>>();
			Animation_->FromJson(os["Animation"]);
			SetFunction();
		}

		/**
		 * @brief		�֐���ݒ�
		 */
		void SetFunction() {
			if (ParamName_ == "X") { OnGetSetX<Obj, Type> func(OnGet, OnSet); }
			else if (ParamName_ == "Y"){ OnGetSetY<Obj, Type> func(OnGet, OnSet); }
			else if (ParamName_ == "Z"){ OnGetSetZ<Obj, Type> func(OnGet, OnSet); }
			else if (ParamName_ == "Alpha") { OnGetSetAlpha<Obj, Type> func(OnGet, OnSet); }
		}
	};
}