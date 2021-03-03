#pragma once

#include	"Common.h"

namespace Sample {

	class Buffer
	{
	private:
		GLuint			ID_;
		GLenum			Type_;

	public:
		/**
		 * @brief		�R���X�g���N�^
		 */
		Buffer();
		/**
		 * @brief		�f�X�g���N�^
		 */
		virtual ~Buffer();

		/**
		 * @brief		�o�b�t�@�̐���
		 * @param[in]	type	�o�b�t�@�̎��
		 * @param[in]	size	�o�b�t�@�T�C�Y
		 * @param[in]	data	�o�b�t�@�Ɋi�[����f�[�^
		 * @param[in]	usage	�o�b�t�@�̎g�p�@
		 * @return		true	��������
		 *				false	�������s
		 */
		bool Create(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage);
		/**
		 * @brief		�o�b�t�@�ւ̃f�[�^��������
		 * @param[in]	offset	�������݃I�t�Z�b�g
		 * @param[in]	size	�������݃T�C�Y
		 * @param[in]	data	�o�b�t�@�Ɋi�[����f�[�^
		 * @return		true	�������ݐ���
		 *				false	�������ݎ��s
		 */
		bool SubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);

		const GLuint ID() const noexcept { return ID_; }
	};

}