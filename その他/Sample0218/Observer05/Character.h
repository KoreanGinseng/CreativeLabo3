#pragma once

#include	"Texture.h"
#include	"Subject.h"

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

	/** �c�@�ύX�ʒm */
	Subject<int> lifeSubject;
	/** �R�C���ύX�ʒm */
	Subject<int> coinSubject;
	/** �X�R�A�ύX�ʒm */
	Subject<int> scoreSubject;
public:
	/**
	 * �R���X�g���N�^
	 */
	Character()
		: life(3)
		, coin(0)
		, score(0)
		, texture()
		, lifeSubject()
		, coinSubject()
		, scoreSubject()
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
		return texture.Load("Player.png");
	}

	/**
	 * @brief	���S
	 *			�c�@�����炷
	 */
	void Dead()
	{
		life -= 1;
		lifeSubject.Notify(life);
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
		coinSubject.Notify(coin);
		scoreSubject.Notify(score);
		std::cout << "�v���C���[���R�C�����l��" << std::endl;
	}

	/**
	 * @brief	�G��|����
	 *			�X�R�A��200���₷
	 */
	void KillEnemy()
	{
		score += 200;
		scoreSubject.Notify(score);
		std::cout << "�v���C���[���G��|����" << std::endl;
	}

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
	IObservable<int>* GetLifeSubject() { return &lifeSubject; }
	IObservable<int>* GetCoinSubject() { return &coinSubject; }
	IObservable<int>* GetScoreSubject() { return &scoreSubject; }
};

