#include "Buffer.h"

using namespace Sample;

/**
 * @brief		�R���X�g���N�^
 */
Buffer::Buffer()
: ID_(0) 
, Type_(0) {
}

/**
 * @brief		�f�X�g���N�^
 */
Buffer::~Buffer() {
	glDeleteBuffers(1, &ID_);
}

/**
 * @brief		�o�b�t�@�̐���
 * @param[in]	type	�o�b�t�@�̎��
 * @param[in]	size	�o�b�t�@�T�C�Y
 * @param[in]	data	�o�b�t�@�Ɋi�[����f�[�^
 * @param[in]	usage	�o�b�t�@�̎g�p�@
 * @return		true	��������
 *				false	�������s
 */
bool Buffer::Create(GLenum type, GLsizeiptr size, const GLvoid* data, GLenum usage) {
	Type_ = type;
	glGenBuffers(1, &ID_);
	glBindBuffer(Type_, ID_);
	glBufferData(Type_, size, data, usage);
	glBindBuffer(Type_, 0);
	GL_ERROR_RETURN("�o�b�t�@�̍쐬�Ɏ��s...", false);
	return true;
}

/**
 * @brief		�o�b�t�@�ւ̃f�[�^��������
 * @param[in]	offset	�������݃I�t�Z�b�g
 * @param[in]	size	�������݃T�C�Y
 * @param[in]	data	�o�b�t�@�Ɋi�[����f�[�^
 * @return		true	�������ݐ���
 *				false	�������ݎ��s
 */
bool Buffer::SubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) {
	glBindBuffer(Type_, ID_);
	glBufferSubData(Type_, offset, size, data);
	glBindBuffer(Type_, 0);
	GL_ERROR_RETURN("�o�b�t�@�ւ̃f�[�^�]���Ɏ��s...", false);
	return true;
}
