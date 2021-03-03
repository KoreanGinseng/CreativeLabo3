#pragma once

#include	"FrameTimer.h"
#include	"JsonUtillity.h"

namespace Sample {
	enum class InterpolationType {
		Nearest,
		Linear,
	};
	template <class...>
	using void_t = void;

	template< class, class = void >
	struct is_float_multiple : std::false_type
	{};

	template< class T >
	struct is_float_multiple<T,
		void_t<decltype(std::declval<T&>() * std::declval<float&>())>
	> : std::true_type
	{};

	template<typename T>
	class InterpolationValue {
	public:
		virtual T GetValue(const T& v1, const T& v2, float p) const = 0;
		virtual InterpolationType GetType() const = 0;
	};

	template<typename T>
	class NearestInterpolation : public InterpolationValue< T > {
	public:
		T GetValue(const T& v1, const T& v2, float p) const override {
			return v2;
		}
		InterpolationType GetType() const override {
			return InterpolationType::Nearest;
		}
	};

	template<typename T>
	class LinearInterpolation : public InterpolationValue< T > {
	public:
		T GetValue(const T& v1, const T& v2, float p) const override {
			return v1 + (v2 - v1) * p;
		}
		InterpolationType GetType() const override {
			return InterpolationType::Linear;
		}
	};

	template<typename T>
	class KeyFrameAnimation {
	public:
		struct KeyFrame {
			/** 時間 */
			float		Time;
			/** パラメーター */
			T			Value;
		};
		struct Animation {
			/** ループフラグ */
			bool loop;

			/** アニメーションフレーム */
			std::vector< KeyFrame > Frames;
		};
		using AnimationPtr = std::shared_ptr< Animation >;

	private:
		/** アニメーション */
		AnimationPtr								Animation_;
		/** 補間計算 */
		std::shared_ptr< InterpolationValue<T> >	Interpolation_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		KeyFrameAnimation()
		: Animation_() 
		, Interpolation_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		KeyFrameAnimation(const Animation& anim, std::shared_ptr< InterpolationValue<T> > ip)
		: Animation_(std::make_shared<Animation>(anim))
		, Interpolation_(ip) {
		}
		/**
		 * @brief		デストラクタ
		 */
		~KeyFrameAnimation() {
		}

		/**
		 * @brief		値の取得
		 * @param[in]	t		時間
		 */
		const T GetValue(float t) const {
			float at = t;
			if (at >= Animation_->Frames.back().Time)
			{
				if (Animation_->loop)
				{
					at = fmodf(at, Animation_->Frames.back().Time);
				}
				else
				{
					return Animation_->Frames.back().Value;
				}
			}
			int size = Animation_->Frames.size();
			for (int i = 0;i < size;i++)
			{
				auto& fm = Animation_->Frames[i];
				if (at < fm.Time)
				{
					if (i == 0 || !Interpolation_)
					{
						return fm.Value;
					}
					else
					{
						auto& pf = Animation_->Frames[i - 1];
						float ft = (at - pf.Time) / (fm.Time - pf.Time);
						return Interpolation_->GetValue(Animation_->Frames[i - 1].Value, fm.Value, ft);
					}
				}
			}
			return Animation_->Frames.back().Value;
		}

		/**
		 * @brief		独自のＪＳＯＮ出力実装
		 */
		void ToJson(nlohmann::json& os)
		{
			os["Loop"] = Animation_->loop;

			int it = (int)Interpolation_->GetType();
			os["Interpolation"] = it;

			int size = Animation_->Frames.size();
			nlohmann::json& osf = os["Frames"];
			for (int i = 0; i < size; i++)
			{
				auto& fm = Animation_->Frames[i];
				osf[i]["Time"] = fm.Time;
				JsonUtillity<T>::ToJson(fm.Value, osf[i]["Value"]);
			}
		}
		/**
		 * @brief		独自のＪＳＯＮ読み込み実装
		 */
		void FromJson(nlohmann::json& os)
		{
			Animation_ = std::make_shared<Animation>();
			os["Loop"].get_to(Animation_->loop);

			int it;
			os["Interpolation"].get_to(it);

			nlohmann::json& osf = os["Frames"];
			int size = osf.size();
			Animation_->Frames.resize(size);
			for (int i = 0; i < size; i++)
			{
				auto& fm = Animation_->Frames[i];
				osf[i]["Time"].get_to(fm.Time);
				JsonUtillity<T>::FromJson(fm.Value, osf[i]["Value"]);
			}
			CreateInterpolation((InterpolationType)it);
		}

		/**
		 * @brief		補間機能の生成
		 */
		template< bool bp = is_float_multiple<T>::value >
		void CreateInterpolation(InterpolationType t)
		{
			switch (t)
			{
			case InterpolationType::Nearest:
				Interpolation_ = std::make_shared< NearestInterpolation< T > >();
				break;
			case InterpolationType::Linear:
				Interpolation_ = std::make_shared< LinearInterpolation< T > >();
				break;
			}
		}

		/**
		 * @brief		時間で補間できない場合の特殊化
		 */
		template< >
		void CreateInterpolation<false>(InterpolationType t)
		{
			Interpolation_ = std::make_shared< NearestInterpolation< T > >();
		}
	};
}