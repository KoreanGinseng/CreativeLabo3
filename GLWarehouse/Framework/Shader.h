#pragma once

#include	"ResourceBase.h"

namespace Sample {

	class Shader : public ResourceBase
	{
	private:
		GLuint		ID_;
		GLint		matVP_;
		GLint		matModel_;
		GLint		texColor_;

		GLuint Compile(GLenum type, const char* fname);
	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		Shader();
		/**
		 * @brief		�R���X�g���N�^
		 * @param[in]	name	�V�F�[�_�[(���_�V�F�[�_�[/�t���O�����g�V�F�[�_�[�Ŗ��O�w��)
		 */
		Shader(const char* name);
		/**
		 * @brief		�R���X�g���N�^
		 * @param[in]	vname	���_�V�F�[�_�[
		 * @param[in]	fname	�t���O�����g�V�F�[�_�[
		 */
		Shader(const char* vname, const char* fname);
		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~Shader();

		/**
		 * @brief		�V�F�[�_�[�̐���
		 * @param[in]	vname	���_�V�F�[�_�[
		 * @param[in]	fname	�t���O�����g�V�F�[�_�[
		 * @return		true	��������
		 *				false	�������s
		 */
		bool Load(const char* vname, const char* fname);
		/**
		 * @brief		�V�F�[�_�[�̗L����
		 * @return		true	����
		 *				false	���s
		 */
		bool Enable();

		/**
		 * @brief		�������
		 */
		void Release();

		bool BindTexture(GLuint no, GLuint id);
		bool SetViewProjectionMatrix(const float* fv);
		bool SetModelMatrix(const float* fv);

		GLint GetUniformLocation(const char* name);

		const GLuint ID() const noexcept { return ID_; }
	};
	using ShaderPtr = std::shared_ptr< Shader >;

}