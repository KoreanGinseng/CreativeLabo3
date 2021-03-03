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
		 * @brief		�R���X�g���N�^
		 */
		FrameBuffer();

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~FrameBuffer();

		/**
		 * @brief		����
		 * @param[in]	w		��
		 * @param[in]	h		����
		 * @return		true	����
		 *				false	���s
		 */
		bool Create(const int w, const int h);

		/**
		 * @brief		�o�C���h
		 * @return		true	����
		 *				false	���s
		 */
		bool Bind();

		/**
		 * @brief		�������
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