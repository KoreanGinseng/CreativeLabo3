#pragma once

#include	"Texture.h"

/**
 * �L�����N�^�[�N���X
 */
class Character
{
private:
	/** �L�����N�^�[�̎c�@ */
	int life;
	/** �L�����N�^�[�̃R�C�� */
	int coin;
	/** �L�����N�^�[�̃X�R�A */
	int score;

	/** �L�����N�^�[�͉摜������ */
	Texture texture;
	/** �X�R�A�p�摜 */
	Texture scoreTexture;
	/** �R�C���p�摜 */
	Texture coinTexture;
	/** ���C�t�p�摜 */
	Texture lifeTexture;
public:
	/**
	 * �R���X�g���N�^
	 */
	Character()
		: life(3)
		, coin(0)
		, score(0)
		, texture()
		, scoreTexture()
		, coinTexture()
		, lifeTexture()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~Character()
	{
	}

	/**
	 * @brief		�摜�̓ǂݍ��ݏ���
	 */
	bool Load()
	{
		if (!texture.Load("Player.png")) { return false; }
		if (!scoreTexture.Load("Score.png")) { return false; }
		if (!coinTexture.Load("Coin.png")) { return false; }
		if (!lifeTexture.Load("Life.png")) { return false; }
		return true;
	}

	/**
	 * @brief	���S
	 *			�c�@�����炷
	 */
	void Dead()
	{
		life -= 1;
		std::cout << "�v���C���[������ł��܂���" << std::endl;
	}

	/**
	 * @brief	�R�C���̊l��
	 *			�R�C����1�����₵�āA�X�R�A��100���₷
	 */
	void AcquiredCoin()
	{
		coin += 1;
		score += 100;
		std::cout << "�v���C���[���R�C�����l��" << std::endl;
	}

	/**
	 * @brief	�G��|����
	 *			�X�R�A��200���₷
	 */
	void KillEnemy()
	{
		score += 200;
		std::cout << "�v���C���[���G��|����" << std::endl;
	}

	/**
	 * @brief	�X�R�A��\������
	 */
	void PrintScore() const {
		std::cout << scoreTexture.GetName() << " : " << score << std::endl;
	}
	/**
	 * @brief	�R�C����\������
	 */
	void PrintCoin() const {
		std::cout << coinTexture.GetName() << " : " << coin << std::endl;
	}
	/**
	 * @brief	���C�t��\������
	 */
	void PrintHP() const {
		std::cout << lifeTexture.GetName() << " : " << life << std::endl;
	}

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
};

