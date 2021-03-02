#pragma once

#include	"Texture.h"

/**
 * �R�C��UI�N���X
 */
class CoinUI
{
private:
	/** �R�C�� */
	int coin;
	/** �R�C���p�摜 */
	Texture texture;
public:
	/**
	 * �R���X�g���N�^
	 */
	CoinUI()
		: coin(0)
		, texture()
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
	void Print() const {
		std::cout << texture.GetName() << " : " << coin << std::endl;
	}

	void SetCoin(int val) { coin = val; }
};

