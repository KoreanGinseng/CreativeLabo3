#pragma once

#include	"Texture.h"
#include	"ReactiveParameter.h"

/**
 * �L�����N�^�[�N���X
 */
class Character
{
private:
	/** �L�����N�^�[�̎c�@ */
	ReactiveParameter<int> life;
	/** �L�����N�^�[�̃R�C�� */
	ReactiveParameter<int> coin;
	/** �L�����N�^�[�̃X�R�A */
	ReactiveParameter<int> score;

	/** �L�����N�^�[�͉摜������ */
	Texture texture;
public:
	/**
	 * �R���X�g���N�^
	 */
	Character()
		: life(3)
		, coin(0)
		, score(0)
		, texture()
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

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
	IObservable<int>* GetLifeSubject() { return &life; }
	IObservable<int>* GetCoinSubject() { return &coin; }
	IObservable<int>* GetScoreSubject() { return &score; }
};

