#pragma once

#include	"Character.h"

/**
 * �X�R�AUI�N���X
 */
class ScoreUI
{
private:
	/** �X�R�A�p�摜 */
	Texture texture;
public:
	/**
	 * �R���X�g���N�^
	 */
	ScoreUI()
		: texture()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~ScoreUI()
	{
	}

	/**
	 * @brief		�摜�̓ǂݍ��ݏ���
	 */
	bool Load()
	{
		return texture.Load("Score.png");
	}

	/**
	 * @brief	�X�R�A��\������
	 */
	void Print(const Character& chara) const {
		std::cout << texture.GetName() << " : " << chara.GetScore() << std::endl;
	}
};

