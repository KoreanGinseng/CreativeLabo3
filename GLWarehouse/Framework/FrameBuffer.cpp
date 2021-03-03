#include "FrameBuffer.h"

using namespace Sample;

/**
 * @brief		�R���X�g���N�^
 */
FrameBuffer::FrameBuffer()
: ID_(0)
, Width_(0)
, Height_(0)
, Color_() 
, Depth_() {
}

/**
 * @brief		�f�X�g���N�^
 */
FrameBuffer::~FrameBuffer() {
	Release();
}

/**
 * @brief		����
 * @param[in]	w		��
 * @param[in]	h		����
 * @return		true	����
 *				false	���s
 */
bool FrameBuffer::Create(const int w, const int h) {
	//�T�C�Y�ۑ�
	Width_ = w;
	Height_ = h;
	// �t���[���o�b�t�@�I�u�W�F�N�g�A�f�v�X�ƃJ���[�p�̃e�N�X�`�����쐬
	glGenFramebuffersEXT(1, &ID_);
	GL_ERROR_RETURN("�t���[���o�b�t�@�̍쐬�Ɏ��s...", false);
	Color_ = std::make_shared<Texture>();
	Color_->Create(w, h, Texture::FormatType::R8G8B8A8);
	Depth_ = std::make_shared<Texture>();
	Depth_->Create(w, h, Texture::FormatType::Depth);

	// �e�N�X�`�����t���[���o�b�t�@�I�u�W�F�N�g�Ƀo�C���h
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ID_);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, Color_->ID(), 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, Depth_->ID(), 0);
	GL_ERROR_RETURN("�t���[���o�b�t�@�̃o�C���h�Ɏ��s...", false);

	// �f�t�H���g�̃����_�[�^�[�Q�b�g�ɖ߂�
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}

/**
 * @brief		�o�C���h
 * @return		true	����
 *				false	���s
 */
bool FrameBuffer::Bind() {
	// �o�b�t�@��ύX
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, ID_);
	GL_ERROR_RETURN("�t���[���o�b�t�@�̃o�C���h�Ɏ��s...", false);
	glViewport(0, 0, Width_, Height_);
	return true;
}

/**
 * @brief		�������
 */
void FrameBuffer::Release() {
	if (ID_ == 0)
	{
		return;
	}
	glDeleteFramebuffersEXT(1, &ID_);
	ID_ = 0;
	Color_->Release();
	Depth_->Release();
}