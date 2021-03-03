#pragma once

#include	"Texture.h"

namespace Sample {

	class FrameBuffer
	{
	private:
		GLuint			ID_;
		GLint			Width_;
		GLint			Height_;
		TexturePtr		Color_;
		TexturePtr		Depth_;
	public:
		/**
		 * @brief		コンストラクタ
		 */
		FrameBuffer();

		/**
		 * @brief		デストラクタ
		 */
		virtual ~FrameBuffer();

		/**
		 * @brief		生成
		 * @param[in]	w		幅
		 * @param[in]	h		高さ
		 * @return		true	成功
		 *				false	失敗
		 */
		bool Create(const int w, const int h);

		/**
		 * @brief		バインド
		 * @return		true	成功
		 *				false	失敗
		 */
		bool Bind();

		/**
		 * @brief		解放処理
		 */
		void Release();

		const GLuint ID() const noexcept { return ID_; }
		const GLint Width() const noexcept { return Width_; }
		const GLint Height() const noexcept { return Height_; }
		const TexturePtr Color() const { return Color_; }
		const TexturePtr Depth() const { return Depth_; }
	};
	using FrameBufferPtr = std::shared_ptr< FrameBuffer >;

}