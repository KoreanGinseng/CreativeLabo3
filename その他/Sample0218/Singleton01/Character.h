#pragma once

#include	<iostream>

/**
 * �L�����N�^�[�N���X
 */
class Character
{
private:
	/** �L�����N�^�[ID */
	int id;
	/** �L�����N�^�[�̎c�@ */
	int life;
	/** �L�����N�^�[�̃R�C�� */
	int coin;
	/** �L�����N�^�[�̃X�R�A */
	int score;
public:
	/**
	 * �R���X�g���N�^
	 */
	Character(int id)
		: id(id)
		, life(3)
		, coin(0)
		, score(0)
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~Character()
	{
	}

	/**
	 * @brief	���S
	 *			�c�@�����炷
	 */
	void Dead()
	{
		life -= 1;
		std::cout << "�v���C���[[" << id << "]������ł��܂���" << std::endl;
	}

	/**
	 * @brief	�R�C���̊l��
	 *			�R�C����1�����₵�āA�X�R�A��100���₷
	 */
	void AcquiredCoin()
	{
		coin += 1;
		score += 100;
		std::cout << "�v���C���[[" << id << "]���R�C�����l��" << std::endl;
	}

	/**
	 * @brief	�G��|����
	 *			�X�R�A��200���₷
	 */
	void KillEnemy()
	{
		score += 200;
		std::cout << "�v���C���[[" << id << "]���G��|����" << std::endl;
	}

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
};

