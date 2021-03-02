#pragma once

#include	<iostream>

/**
 * �摜��ێ�����N���X
 */
class Texture
{
private:
	/** �摜�t�@�C���� */
	std::string fileName;
public:
	/**
	 * �R���X�g���N�^
	 */
	Texture()
		: fileName()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~Texture()
	{
	}

	/**
	 * @brief		�摜�̓ǂݍ��ݏ���
	 * @param[in]	fname		�t�@�C����
	 */
	bool Load(const std::string& fname)
	{
		std::cout << "�摜[" << fname << "]��ǂݍ���!!" << std::endl;
		fileName = fname;
		return true;
	}

	const std::string& GetName() const { return fileName; }
};

