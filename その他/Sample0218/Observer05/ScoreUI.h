#pragma once

#include	"Texture.h"
#include	"Observer.h"

/**
 * �X�R�AUI�N���X
 */
class ScoreUI
{
private:
	/** �X�R�A */
	int score;
	/** �X�R�A�p�摜 */
	Texture texture;
public:
	/**
	 * �R���X�g���N�^
	 */
	ScoreUI()
		: score(0)
		, texture()
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
	void Print() const {
		std::cout << texture.GetName() << " : " << score << std::endl;
	}

	void SetScore(int val) { score = val; }
};

