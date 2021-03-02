#pragma once

#include	"Character.h"

/**
 * �c�@UI�N���X
 */
class LifeUI
{
private:
	/** �c�@�p�摜 */
	Texture texture;
public:
	/**
	 * �R���X�g���N�^
	 */
	LifeUI()
		: texture()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~LifeUI()
	{
	}

	/**
	 * @brief		�摜�̓ǂݍ��ݏ���
	 */
	bool Load()
	{
		return texture.Load("Life.png");
	}

	/**
	 * @brief	�c�@��\������
	 */
	void Print(const Character& chara) const {
		std::cout << texture.GetName() << " : " << chara.GetLife() << std::endl;
	}
};

