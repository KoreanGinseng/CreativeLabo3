#pragma once

#include	"ResourceBase.h"

namespace Sample {

	class Texture : public ResourceBase
	{
	public:
		enum class FormatType {
			R8G8B8,
			R8G8B8A8,
			Depth,
			Unknown,
		};
	protected:
		GLuint			ID_;
		GLint			Width_;
		GLint			Height_;
		FormatType		Format_;
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		Texture();

		/**
		 * @brief		�R���X�g���N�^
		 *				�����Ɠ����ɓǂݍ��݂������Ȃ�
		 * @param[in]	name	�ǂݍ��ރe�N�X�`����
		 */
		Texture(const char* name);

		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~Texture();

		/**
		 * @brief		�ǂݍ���
		 * @param[in]	name	�ǂݍ��ރe�N�X�`����
		 * @return		true	�ǂݍ��ݐ���
		 *				false	�ǂݍ��ݎ��s
		 */
		bool Load(const char* name);

		/**
		 * @brief		����
		 * @param[in]	w		��
		 * @param[in]	h		����
		 * @param[in]	format	�t�H�[�}�b�g
		 * @return		true	����
		 *				false	���s
		 */
		bool Create(const int w, const int h, const FormatType format);

		/**
		 * @brief		�������
		 */
		void Release();

		const GLuint ID() const noexcept { return ID_; }
		const GLint Width() const noexcept { return Width_; }
		const GLint Height() const noexcept { return Height_; }

	};
	using TexturePtr = std::shared_ptr< Texture >;

}