#include "Texture.h"

using namespace Sample;

/**
 * @brief		�R���X�g���N�^
 */
Texture::Texture()
: ResourceBase()
, ID_(0)
, Width_(0) 
, Height_(0) 
, Format_(FormatType::Unknown) {
}

/**
 * @brief		�R���X�g���N�^
 *				�����Ɠ����ɓǂݍ��݂������Ȃ�
 * @param[in]	name	�ǂݍ��ރe�N�X�`����
 */
Texture::Texture(const char* name)
: ResourceBase()
, ID_(0)
, Width_(0)
, Height_(0)
, Format_(FormatType::Unknown) {
	Load(name);
}

/**
 * @brief		�f�X�g���N�^
 */
Texture::~Texture() {
	Release();
}

/**
 * @brief		�ǂݍ���
 * @param[in]	name	�ǂݍ��ރe�N�X�`����
 * @return		true	�ǂݍ��ݐ���
 *				false	�ǂݍ��ݎ��s
 */
bool Texture::Load(const char* name) {
	//png�摜�t�@�C���̃��[�h
	png_structp sp = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	png_infop   ip = png_create_info_struct(sp);
	FILE* fp = fopen(name, "rb");
	if (!fp)
	{
		ERROR_LOG("�e�N�X�`��" << name << "���J���܂���");
		return false;
	}
	png_init_io(sp, fp);
	png_read_info(sp, ip);
	int ColorType;
	int bpp;
	png_get_IHDR(sp, ip, (png_uint_32*)&Width_, (png_uint_32*)&Height_, &bpp, &ColorType, nullptr, nullptr, nullptr);
	//����ϊ����čX�V
	png_set_expand(sp);
	if (bpp > 8)
	{
		png_set_strip_16(sp);
	}
	if (ColorType == PNG_COLOR_TYPE_GRAY)
	{
		png_set_gray_to_rgb(sp);
	}
	png_read_update_info(sp, ip);
	//�`�����l�����ݒ�
	png_byte bits = png_get_channels(sp, ip);
	// �������̈�m��
	int rb = png_get_rowbytes(sp, ip);
	GLubyte* data = new GLubyte[Height_ * rb];
	GLubyte** recv = new GLubyte*[Height_];
	for (int i = 0; i < Height_; i++)
	{
		recv[i] = &data[i * rb];
	}
	//�ǂݍ���
	png_read_image(sp, recv);
	png_read_end(sp, ip);
	png_destroy_read_struct(&sp, &ip, NULL);
	fclose(fp);
	delete[] recv;
	fclose(fp);

	//�t�H�[�}�b�g�𒲂ׂ�
	int iFormat = GL_RGBA;
	int Format = GL_RGBA;
	int Type = GL_UNSIGNED_BYTE;
	switch (bits)
	{
	case 1:
		Format_ = FormatType::Unknown;
		iFormat = GL_LUMINANCE;
		Format = GL_LUMINANCE;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case 2:
		Format_ = FormatType::Unknown;
		Type = GL_UNSIGNED_SHORT_4_4_4_4;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
		break;
	case 3:
		Format_ = FormatType::R8G8B8;
		iFormat = GL_RGB;
		Format = GL_RGB;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case 4:
		Format_ = FormatType::R8G8B8A8;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		break;
	}
	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, ID_);
	glTexImage2D(GL_TEXTURE_2D, 0, iFormat, Width_, Height_, 0, Format, Type, data);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// �e�N�X�`���̃p�����[�^��ݒ肷��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] data;
	Name_ = name;
	INFO_LOG("�e�N�X�`��" << name << "��ǂݍ���...");
	return true;
}

/**
 * @brief		����
 * @param[in]	w		��
 * @param[in]	h		����
 * @param[in]	format	�t�H�[�}�b�g
 * @return		true	����
 *				false	���s
 */
bool Texture::Create(const int w, const int h, const FormatType format) {
	//�p�����[�^�[��ۑ�
	Width_ = w;
	Height_ = h;
	Format_ = format;
	//����
	glGenTextures(1, &ID_);
	glBindTexture(GL_TEXTURE_2D, ID_);
	switch (Format_)
	{
	case FormatType::R8G8B8:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width_, Height_, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		break;

	case FormatType::R8G8B8A8:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width_, Height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		break;

	case FormatType::Depth:
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width_, Height_, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		break;
	}
	// �e�N�X�`���̃p�����[�^��ݒ肷��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindTexture(GL_TEXTURE_2D, 0);
	GL_ERROR_RETURN("�e�N�X�`���̐����Ɏ��s...", false);
	Name_ = "FrameBuffer";
	return true;
}

/**
 * @brief		�������
 */
void Texture::Release() {
	if (ID_ == 0)
	{
		return;
	}
	glDeleteTextures(1, &ID_);
	ID_ = 0;
	Width_ = 0;
	Height_ = 0;
	INFO_LOG("�e�N�X�`��" << Name_ << "�����...");
	Name_ = "";
}