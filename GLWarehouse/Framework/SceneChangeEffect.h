#pragma once

#include	"Scene.h"

namespace Sample {

	class __declspec(novtable) ISceneChangeEffect
	{
	public:
		/**
		 * @brief		���ʓK�p�`��
		 */
		virtual void Render(SceneList& list, float t) = 0;
		/**
		 * @brief		�I�����f
		 * @param[in]	t		����
		 * @return		true	�I�����Ă���
		 *				false	�I�����Ă��Ȃ�
		 */
		virtual bool IsEnd(const float t) const = 0;
	};
	using SceneChangeEffectPtr = std::shared_ptr< ISceneChangeEffect >;

	/**
	 * @brief			�t�F�[�h
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
		 * @brief		�R���X�g���N�^
		 */
		SceneChangeFade(float st, float et, Type type)
		: StartTime_(st)
		, EndTime_(et)
		, StartAlpha_(type == Type::In ? 0.0f : 1.0f)
		, EndAlpha_(type == Type::In ? 1.0f : 0.0f)
		{
		}
		/**
		 * @brief		�f�X�g���N�^
		 */
		~SceneChangeFade()
		{
		}

		/**
		 * @brief		���ʓK�p�`��
		 */
		void Render(SceneList& list, float t) override
		{
			//�o�ߊ���
			float par = (t - StartTime_) / (EndTime_ - StartTime_);
			if (par < 0) {		par = 0; }
			else if (par < 0) {	par = 1; }
			//�A���t�@�Z�o
			float a = StartAlpha_ + (EndAlpha_ - StartAlpha_) * par;
			//�`����s
			std::for_each(list.begin(), list.end(), [&](const ScenePtr& ptr)
				{
					SpritePtr sp = ptr->FrameSprite();
					sp->Alpha(a);
					sp->Render();
				}
			);
		}

		/**
		 * @brief		�I�����f
		 * @param[in]	t		����
		 * @return		true	�I�����Ă���
		 *				false	�I�����Ă��Ȃ�
		 */
		bool IsEnd(const float t) const override {
			return t >= EndTime_;
		}

		/**
		 * @brief		�ύX�G�t�F�N�g�̐���
		 * @param[in]	st		�J�n����
		 * @param[in]	et		�I������
		 * @param[in]	type	�t�F�[�h��ށiFadeType::In or FadeType::Out�j
		 */
		static SceneChangeEffectPtr Create(float st, float et, Type type) {
			return std::make_shared< SceneChangeFade>(st, et, type);
		}
	};
	
	/**
	 * @brief			�X���C�h
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
		 * @brief		�R���X�g���N�^
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
		 * @brief		�f�X�g���N�^
		 */
		~SceneChangeSlide()
		{
		}

		/**
		 * @brief		���ʓK�p�`��
		 */
		void Render(SceneList& list, float t) override
		{
			//�o�ߊ���
			float par = (t - StartTime_) / (EndTime_ - StartTime_);
			if (par < 0) {		par = 0; }
			else if (par < 0) {	par = 1; }
			//���W�Z�o
			Vector3F pos = StartPosition_ + (EndPosition_ - StartPosition_) * par;
			//�`����s
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
		 * @brief		�I�����f
		 * @param[in]	t		����
		 * @return		true	�I�����Ă���
		 *				false	�I�����Ă��Ȃ�
		 */
		bool IsEnd(const float t) const override {
			return t >= EndTime_;
		}

		/**
		 * @brief		�ύX�G�t�F�N�g�̐���
		 * @param[in]	st		�J�n����
		 * @param[in]	et		�I������
		 * @param[in]	type	�X���C�h���
		 */
		static SceneChangeEffectPtr Create(float st, float et, Type type) {
			return std::make_shared< SceneChangeSlide >(st, et, type);
		}
	};

	/**
	 * @brief			�X���C�h
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
		 * @brief		�R���X�g���N�^
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
		 * @brief		�f�X�g���N�^
		 */
		~SceneChangeZoom()
		{
		}

		/**
		 * @brief		���ʓK�p�`��
		 */
		void Render(SceneList& list, float t) override
		{
			//�o�ߊ���
			float par = (t - StartTime_) / (EndTime_ - StartTime_);
			if (par < 0) { par = 0; }
			else if (par < 0) { par = 1; }
			//�X�P�[���Z�o
			float s = StartScale_ + (EndScale_ - StartScale_) * par;
			//�`����s
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
		 * @brief		�I�����f
		 * @param[in]	t		����
		 * @return		true	�I�����Ă���
		 *				false	�I�����Ă��Ȃ�
		 */
		bool IsEnd(const float t) const override {
			return t >= EndTime_;
		}

		/**
		 * @brief		�ύX�G�t�F�N�g�̐���
		 * @param[in]	st		�J�n����
		 * @param[in]	et		�I������
		 * @param[in]	pivot	��ʒu
		 * @param[in]	type	�Y�[�����
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
		 * @brief		�R���X�g���N�^
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
		 * @brief		�f�X�g���N�^
		 */
		virtual ~SceneChange() = default;

		/**
		 * @brief		�X�V
		 */
		virtual void Update()
		{
			//�o�ߎ��ԉ��Z
			NowTime_ += FrameTimer::Instance().TimeF();
		}

		/**
		 * @brief		�`��
		 */
		virtual void Render()
		{
			//�G�t�F�N�g�K�p���ĕ`��
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
		 * @brief		���
		 */
		virtual void Release()
		{
			std::for_each(PrevScenes_.begin(), PrevScenes_.end(), [](const ScenePtr& ptr) { ptr->Destroy(); });
		}

		/**
		 * @brief		�I�����f
		 * @param[in]	t		����
		 * @return		true	�I�����Ă���
		 *				false	�I�����Ă��Ȃ�
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