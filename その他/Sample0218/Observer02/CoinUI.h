#pragma once

#include	"Character.h"

/**
 * �R�C��UI�N���X
 */
class CoinUI
{
private:
	/** �R�C���p�摜 */
	Texture texture;
public:
	/**
	 * �R���X�g���N�^
	 */
	CoinUI()
		: texture()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~CoinUI()
	{
	}

	/**
	 * @brief		�摜�̓ǂݍ��ݏ���
	 */
	bool Load()
	{
		return texture.Load("Coin.png");
	}

	/**
	 * @brief	�R�C����\������
	 */
	void Print(const Character& chara) const {
		std::cout << texture.GetName() << " : " << chara.GetCoin() << std::endl;
	}
};

