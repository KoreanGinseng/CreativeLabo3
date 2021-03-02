#pragma once

#include	<iostream>
#include	"Score.h"

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
public:
	/**
	 * �R���X�g���N�^
	 */
	Character(int id)
		: id(id)
		, life(3)
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
		Score::GetInstance().AddCoin(1);
		Score::GetInstance().AddScore(100);
		std::cout << "�v���C���[[" << id << "]���R�C�����l��" << std::endl;
	}

	/**
	 * @brief	�G��|����
	 *			�X�R�A��200���₷
	 */
	void KillEnemy()
	{
		Score::GetInstance().AddScore(200);
		std::cout << "�v���C���[[" << id << "]���G��|����" << std::endl;
	}

	int GetLife() const { return life; }
};

