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
		 * @brief		コンストラクタ
		 */
		TextureRender()
		: RenderBase()
		, Sprite_() 
		, Animator_(){
		}
		/**
		 * @brief		コピーコンストラクタ
		 */
		TextureRender(const TextureRender& act)
		: RenderBase(act)
		, Sprite_(act.Sprite_) 
		, Animator_(act.Animator_) {
		}

		/**
		 * @brief		デストラクタ
		 */
		~TextureRender() override {
		}

		/**
		 * @brief		初期化
		 */
		void Initialize() {
		}

		/**
		 * @brief		初期化
		 * @param[in]	t	使うテクスチャ
		 * @param[in]	s	使うシェーダー
		 */
		void Initialize(const TexturePtr t, const ShaderPtr s) {
			Sprite_.Create(t, s);
		}

		/**
		 * @brief		開始
		 */
		void Start() override {
			Animator_ = actor->GetAction<SpriteAnimator>();
		}

		/**
		 * @brief		アクターを表示させる
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
		 * @brief		pivotへのAccessor
		 * @param[in]	p	ピボット
		 */
		void Pivot(const Vector3F& p) { Sprite_.Pivot(p); }
		/**
		 * @brief		pivotへのAccessor
		 * @return		ピボット
		 */
		const Vector3F& Pivot() const noexcept { return Sprite_.Pivot(); }

		/**
		 * @brief		anchorへのAccessor
		 * @param[in]	p	アンカー
		 */
		void Anchor(const Vector3F& p) { Sprite_.Anchor(p); }
		/**
		 * @brief		anchorへのAccessor
		 * @return		アンカー
		 */
		const Vector3F& Anchor() const noexcept { return Sprite_.Anchor(); }

		/**
		 * @brief		color_へのAccessor
		 * @param[in]	アルファ値
		 */
		void Alpha(const float a) {
			Sprite_.Alpha(a);
		}

		/**
		 * @brief		color_へのAccessor
		 * @return		アルファ値
		 */
		const float Alpha(void) const {
			return Sprite_.Alpha();
		}

		/**
		 * @brief		独自のＪＳＯＮ出力実装
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
		 * @brief		独自のＪＳＯＮ読み込み実装
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