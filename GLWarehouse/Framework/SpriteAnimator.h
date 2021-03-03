#pragma once

#include	"Collision/Rectangle.h"
#include	"KeyFrameAnimation.h"
#include	"ParserManager.h"

namespace Sample {

	class SpriteAnimator : public ActionBase<SpriteAnimator> {
	protected:

		/** 現在のアニメーション時間 */
		float NowTime_;

		/** 現在のアニメーション矩形 */
		RectangleF SrcRect_;

		using AnimationPtr = std::shared_ptr< KeyFrameAnimation<RectangleF> >;

		/** 実行中アニメーション */
		AnimationPtr									NowAnimation_;
		/** アニメーション */
		std::unordered_map< std::string, AnimationPtr > Animations_;

		/**
		 * @brief		矩形更新
		 */
		void RefreshRect() {
			if (!IsAnimation())
			{
				return;
			}
			SrcRect_ = NowAnimation_->GetValue(NowTime_);
		}
	public:
		/**
		 * @brief		コンストラクタ
		 */
		SpriteAnimator()
		: ActionBase()
		, NowTime_(0)
		, SrcRect_() 
		, NowAnimation_()
		, Animations_() {
		}
		/**
		 * @brief		コンストラクタ
		 */
		SpriteAnimator(const SpriteAnimator& obj)
		: ActionBase(obj)
		, NowTime_(obj.NowTime_)
		, SrcRect_(obj.SrcRect_) 
		, NowAnimation_(obj.NowAnimation_)
		, Animations_(obj.Animations_) {
		}

		/**
		 * @brief		デストラクタ
		 */
		~SpriteAnimator() {
		}

		/**
		 * @brief		アニメーション登録
		 * @param[in]	key		キー
		 * @param[in]	anim	アニメーション
		 */
		void Register(const std::string& key, const KeyFrameAnimation<RectangleF>::Animation& anim) {
			KeyFrameAnimation<RectangleF>::Animation temp = anim;
			float at = 0;
			for (auto& fm : temp.Frames)
			{
				at += fm.Time;
				fm.Time = at;
			}

			auto ap = std::make_shared<KeyFrameAnimation<RectangleF>>(temp, std::make_shared< NearestInterpolation <RectangleF > >());
			Animations_[key] = ap;
		}

		/**
		 * @brief		アニメーション変更
		 * @param[in]	key		キー
		 * @param[in]	same	同一アニメーションの上書き（trueで最初から再生される）
		 * @return		true	アニメーションあり
		 *				false	アニメーションなし
		 */
		bool ChangeAnimation(const std::string& key, bool same = false) {
			auto it = Animations_.find(key);
			if (it == Animations_.end())
			{
				return false;
			}
			//時間のリセット
			if (it->second != NowAnimation_ || same)
			{
				NowTime_ = 0;
			}
			NowAnimation_ = it->second;
			RefreshRect();
			return true;
		}

		/**
		 * @brief		開始
		 */
		void Start() override {
		}

		/**
		 * @brief		更新
		 */
		void Update() override {
			if (IsAnimation())
			{
				NowTime_ += FrameTimer::Instance().TimeF();
				RefreshRect();
			}
		}

		/**
		 * @brief		矩形取得
		 * @return		矩形
		 */
		const RectangleF& SrcRect() const {
			return SrcRect_;
		}

		/**
		 * @brief		矩形設定
		 * @param[in]	矩形
		 */
		void SrcRect(const RectangleF& r) {
			SrcRect_ = r;
		}

		/**
		 * @brief		アニメーションを実行するか取得
		 * @return		true	アニメーションあり
		 *				false	アニメーションなし
		 */
		bool IsAnimation() const {
			return NowAnimation_ != nullptr;
		}

		/**
		 * @brief		独自のＪＳＯＮ出力実装
		 */
		void ToJson(nlohmann::json& os)
		{
			SrcRect_.ToJson(os["SrcRect"]);
			for (auto it : Animations_)
			{
				it.second->ToJson(os[it.first]);
			}
		}
		/**
		 * @brief		独自のＪＳＯＮ読み込み実装
		 */
		void FromJson(nlohmann::json& os)
		{
			SrcRect_.FromJson(os["SrcRect"]);
			for (auto it = os.begin(); it != os.end(); ++it)
			{
				if (it.key() == "SrcRect")
				{
					continue;
				}
				auto p = std::make_shared<KeyFrameAnimation<RectangleF>>();
				p->FromJson(it.value());
				Animations_[it.key()] = p;
			}
		}

		REGISTER_COMPONENTONLY(SpriteAnimator)
	};
	//ポインタ置き換え
	using SpriteAnimatorPtr = std::shared_ptr<SpriteAnimator>;
}