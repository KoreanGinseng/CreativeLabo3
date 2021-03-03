#pragma once

#include	"Sprite.h"
#include	"SpriteAnimator.h"
#include	"ResourceManager.h"

namespace Sample {

	class TextureRender : public RenderBase< TextureRender >{
	protected:
		Sprite Sprite_;
		SpriteAnimatorPtr Animator_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		TextureRender()
		: RenderBase()
		, Sprite_() 
		, Animator_(){
		}
		/**
		 * @brief		�R�s�[�R���X�g���N�^
		 */
		TextureRender(const TextureRender& act)
		: RenderBase(act)
		, Sprite_(act.Sprite_) 
		, Animator_(act.Animator_) {
		}

		/**
		 * @brief		�f�X�g���N�^
		 */
		~TextureRender() override {
		}

		/**
		 * @brief		������
		 */
		void Initialize() {
		}

		/**
		 * @brief		������
		 * @param[in]	t	�g���e�N�X�`��
		 * @param[in]	s	�g���V�F�[�_�[
		 */
		void Initialize(const TexturePtr t, const ShaderPtr s) {
			Sprite_.Create(t, s);
		}

		/**
		 * @brief		�J�n
		 */
		void Start() override {
			Animator_ = actor->GetAction<SpriteAnimator>();
		}

		/**
		 * @brief		�A�N�^�[��\��������
		 */
		void Render() override {
			//Sprite_.Position(actor->Position());
			//Sprite_.Rotation(actor->Rotation());
			//Sprite_.Scale(actor->Scale());
			Sprite_.RefreshMatrix(actor->Matrix());
			if (Animator_)
			{
				Sprite_.SrcRect(Animator_->SrcRect());
			}
			Sprite_.Render();
		}

		/**
		 * @brief		pivot�ւ�Accessor
		 * @param[in]	p	�s�{�b�g
		 */
		void Pivot(const Vector3F& p) { Sprite_.Pivot(p); }
		/**
		 * @brief		pivot�ւ�Accessor
		 * @return		�s�{�b�g
		 */
		const Vector3F& Pivot() const noexcept { return Sprite_.Pivot(); }

		/**
		 * @brief		anchor�ւ�Accessor
		 * @param[in]	p	�A���J�[
		 */
		void Anchor(const Vector3F& p) { Sprite_.Anchor(p); }
		/**
		 * @brief		anchor�ւ�Accessor
		 * @return		�A���J�[
		 */
		const Vector3F& Anchor() const noexcept { return Sprite_.Anchor(); }

		/**
		 * @brief		color_�ւ�Accessor
		 * @param[in]	�A���t�@�l
		 */
		void Alpha(const float a) {
			Sprite_.Alpha(a);
		}

		/**
		 * @brief		color_�ւ�Accessor
		 * @return		�A���t�@�l
		 */
		const float Alpha(void) const {
			return Sprite_.Alpha();
		}

		/**
		 * @brief		�Ǝ��̂i�r�n�m�o�͎���
		 */
		void ToJson(nlohmann::json& os)
		{
			os["Texture"] = Sprite_.Texture()->Name();
			os["Shader"] = Sprite_.Shader()->Name();
			os["Priority"] = Priority_;
			Sprite_.Anchor().ToJson(os["Anchor"]);
			Sprite_.Pivot().ToJson(os["Pivot"]);
		}
		/**
		 * @brief		�Ǝ��̂i�r�n�m�ǂݍ��ݎ���
		 */
		void FromJson(nlohmann::json& os)
		{
			std::string tname;
			std::string sname;
			os["Texture"].get_to(tname);
			os["Shader"].get_to(sname);
			os["Priority"].get_to(Priority_);
			Sprite_.Anchor(Vector3F::FromJsonFile(os["Anchor"]));
			Sprite_.Pivot(Vector3F::FromJsonFile(os["Pivot"]));
			Sprite_.Create(ResourceManager::Instance().GetTextureFromName(tname), ResourceManager::Instance().GetShaderFromName(sname));
		}

		REGISTER_COMPONENTONLY(TextureRender)
	};
}