#pragma once

#include	"Scene.h"

namespace Sample {

	class __declspec(novtable) ISceneChangeEffect
	{
	public:
		/**
		 * @brief		効果適用描画
		 */
		virtual void Render(SceneList& list, float t) = 0;
		/**
		 * @brief		終了判断
		 * @param[in]	t		時間
		 * @return		true	終了している
		 *				false	終了していない
		 */
		virtual bool IsEnd(const float t) const = 0;
	};
	using SceneChangeEffectPtr = std::shared_ptr< ISceneChangeEffect >;

	/**
	 * @brief			フェード
	 */
	class SceneChangeFade : public ISceneChangeEffect
	{
	public:
		enum class Type {
			In,
			Out,
		};
	private:

		float					StartTime_;
		float					EndTime_;
		float					StartAlpha_;
		float					EndAlpha_;

	public:
		/**
		 * @brief		コンストラクタ
		 */
		SceneChangeFade(float st, float et, Type type)
		: StartTime_(st)
		, EndTime_(et)
		, StartAlpha_(type == Type::In ? 0.0f : 1.0f)
		, EndAlpha_(type == Type::In ? 1.0f : 0.0f)
		{
		}
		/**
		 * @brief		デストラクタ
		 */
		~SceneChangeFade()
		{
		}

		/**
		 * @brief		効果適用描画
		 */
		void Render(SceneList& list, float t) override
		{
			//経過割合
			float par = (t - StartTime_) / (EndTime_ - StartTime_);
			if (par < 0) {		par = 0; }
			else if (par < 0) {	par = 1; }
			//アルファ算出
			float a = StartAlpha_ + (EndAlpha_ - StartAlpha_) * par;
			//描画実行
			std::for_each(list.begin(), list.end(), [&](const ScenePtr& ptr)
				{
					SpritePtr sp = ptr->FrameSprite();
					sp->Alpha(a);
					sp->Render();
				}
			);
		}

		/**
		 * @brief		終了判断
		 * @param[in]	t		時間
		 * @return		true	終了している
		 *				false	終了していない
		 */
		bool IsEnd(const float t) const override {
			return t >= EndTime_;
		}

		/**
		 * @brief		変更エフェクトの生成
		 * @param[in]	st		開始時間
		 * @param[in]	et		終了時間
		 * @param[in]	type	フェード種類（FadeType::In or FadeType::Out）
		 */
		static SceneChangeEffectPtr Create(float st, float et, Type type) {
			return std::make_shared< SceneChangeFade>(st, et, type);
		}
	};
	
	/**
	 * @brief			スライド
	 */
	class SceneChangeSlide : public ISceneChangeEffect
	{
	public:
		enum class Type {
			LeftIn,
			RightIn,
			UpIn,
			DownIn,

			LeftOut,
			RightOut,
			UpOut,
			DownOut,
		};
	private:

		float					StartTime_;
		float					EndTime_;
		Vector3F				StartPosition_;
		Vector3F				EndPosition_;

	public:
		/**
		 * @brief		コンストラクタ
		 */
		SceneChangeSlide(float st, float et, Type type)
		: StartTime_(st)
		, EndTime_(et)
		, StartPosition_(
			(type == Type::LeftIn ? (float)-RenderManager::Instance().ScreenWidth() : (type == Type::RightIn ? (float)RenderManager::Instance().ScreenWidth() : 0.0f)),
			(type == Type::UpIn ? (float)-RenderManager::Instance().ScreenHeight() : (type == Type::DownIn ? (float)RenderManager::Instance().ScreenHeight() : 0.0f)),
			0.0f
			)
		, EndPosition_(
			(type == Type::LeftOut ? (float)-RenderManager::Instance().ScreenWidth() : (type == Type::RightOut ? (float)RenderManager::Instance().ScreenWidth() : 0.0f)),
			(type == Type::UpOut ? (float)-RenderManager::Instance().ScreenHeight() : (type == Type::DownOut ? (float)RenderManager::Instance().ScreenHeight() : 0.0f)),
			0.0f
			)
		{
		}
		/**
		 * @brief		デストラクタ
		 */
		~SceneChangeSlide()
		{
		}

		/**
		 * @brief		効果適用描画
		 */
		void Render(SceneList& list, float t) override
		{
			//経過割合
			float par = (t - StartTime_) / (EndTime_ - StartTime_);
			if (par < 0) {		par = 0; }
			else if (par < 0) {	par = 1; }
			//座標算出
			Vector3F pos = StartPosition_ + (EndPosition_ - StartPosition_) * par;
			//描画実行
			std::for_each(list.begin(), list.end(), [&](const ScenePtr& ptr)
				{
					SpritePtr sp = ptr->FrameSprite();
					sp->Position(pos);
					sp->RefreshMatrix();
					sp->Render();
				}
			);
		}

		/**
		 * @brief		終了判断
		 * @param[in]	t		時間
		 * @return		true	終了している
		 *				false	終了していない
		 */
		bool IsEnd(const float t) const override {
			return t >= EndTime_;
		}

		/**
		 * @brief		変更エフェクトの生成
		 * @param[in]	st		開始時間
		 * @param[in]	et		終了時間
		 * @param[in]	type	スライド種類
		 */
		static SceneChangeEffectPtr Create(float st, float et, Type type) {
			return std::make_shared< SceneChangeSlide >(st, et, type);
		}
	};

	/**
	 * @brief			スライド
	 */
	class SceneChangeZoom : public ISceneChangeEffect
	{
	public:
		enum class Type {
			In,
			Out,
		};
	private:

		float					StartTime_;
		float					EndTime_;
		Vector3F				Pivot_;
		float					StartScale_;
		float					EndScale_;

	public:
		/**
		 * @brief		コンストラクタ
		 */
		SceneChangeZoom(float st, float et, Vector3F pivot, Type type)
		: StartTime_(st)
		, EndTime_(et)
		, Pivot_(pivot)
		, StartScale_(type == Type::In ? 0.0f : 1.0f)
		, EndScale_(type == Type::In ? 1.0f : 0.0f)
		{
		}
		/**
		 * @brief		デストラクタ
		 */
		~SceneChangeZoom()
		{
		}

		/**
		 * @brief		効果適用描画
		 */
		void Render(SceneList& list, float t) override
		{
			//経過割合
			float par = (t - StartTime_) / (EndTime_ - StartTime_);
			if (par < 0) { par = 0; }
			else if (par < 0) { par = 1; }
			//スケール算出
			float s = StartScale_ + (EndScale_ - StartScale_) * par;
			//描画実行
			std::for_each(list.begin(), list.end(), [&](const ScenePtr& ptr)
				{
					SpritePtr sp = ptr->FrameSprite();
					sp->Scale(Vector3F(s, s, s));
					sp->Anchor(Pivot_);
					sp->Pivot(Pivot_);
					sp->RefreshMatrix();
					sp->RefreshVertex();
					sp->Render();
				}
			);
		}

		/**
		 * @brief		終了判断
		 * @param[in]	t		時間
		 * @return		true	終了している
		 *				false	終了していない
		 */
		bool IsEnd(const float t) const override {
			return t >= EndTime_;
		}

		/**
		 * @brief		変更エフェクトの生成
		 * @param[in]	st		開始時間
		 * @param[in]	et		終了時間
		 * @param[in]	pivot	基準位置
		 * @param[in]	type	ズーム種類
		 */
		static SceneChangeEffectPtr Create(float st, float et, Vector3F pivot, Type type) {
			return std::make_shared< SceneChangeZoom >(st, et, pivot, type);
		}
	};

	class SceneChange
	{
	private:
		SceneList				PrevScenes_;
		SceneList				NextScenes_;

		SceneChangeEffectPtr	InEffect;
		SceneChangeEffectPtr	OutEffect;

		float					NowTime_;

		SceneChange(const SceneChange& obj) = delete;
		SceneChange& operator=(const SceneChange&) = delete;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		SceneChange(const SceneList prev, const ScenePtr next, const SceneChangeEffectPtr Out, const SceneChangeEffectPtr In)
		: PrevScenes_(prev)
		, NextScenes_() 
		, InEffect(In)
		, OutEffect(Out) 
		, NowTime_(0.0f) {
			NextScenes_.push_back(next);
		}
		/**
		 * @brief		デストラクタ
		 */
		virtual ~SceneChange() = default;

		/**
		 * @brief		更新
		 */
		virtual void Update()
		{
			//経過時間加算
			NowTime_ += FrameTimer::Instance().TimeF();
		}

		/**
		 * @brief		描画
		 */
		virtual void Render()
		{
			//エフェクト適用して描画
			if (OutEffect)
			{
				OutEffect->Render(PrevScenes_, NowTime_);
			}
			else
			{
				std::for_each(PrevScenes_.begin(), PrevScenes_.end(), [](const ScenePtr& ptr) { ptr->Present(); });
			}
			if (InEffect)
			{
				InEffect->Render(NextScenes_, NowTime_);
			}
			else
			{
				std::for_each(NextScenes_.begin(), NextScenes_.end(), [](const ScenePtr& ptr) { ptr->Present(); });
			}
		}

		/**
		 * @brief		解放
		 */
		virtual void Release()
		{
			std::for_each(PrevScenes_.begin(), PrevScenes_.end(), [](const ScenePtr& ptr) { ptr->Destroy(); });
		}

		/**
		 * @brief		終了判断
		 * @param[in]	t		時間
		 * @return		true	終了している
		 *				false	終了していない
		 */
		bool IsEnd() const {
			if (InEffect)
			{
				if (!InEffect->IsEnd(NowTime_))
				{
					return false;
				}
			}
			if (OutEffect)
			{
				if (!OutEffect->IsEnd(NowTime_))
				{
					return false;
				}
			}
			return true;
		}
	};
	using SceneChangePtr = std::shared_ptr< SceneChange >;

}