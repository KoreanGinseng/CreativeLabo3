#pragma once

#include	<iostream>
#include	<cassert>

/**
 * �X�R�A�N���X
 */
class Score
{
private:
	/** �R�C�� */
	int coin;
	/** �X�R�A */
	int score;

	/** �R���X�g���N�^��private�ɂ��ĊO�����琶���ł��Ȃ����� */
	/**
	 * �R���X�g���N�^
	 */
	Score()
		: coin(0)
		, score(0)
	{
	}
	Score(const Score& obj) = delete;
	Score(Score&& obj) = delete;

	/** Singleton�p�C���X�^���X */
	static Score* instance;
public:
	/**
	 * �f�X�g���N�^
	 */
	~Score()
	{
	}

	/**
	 * @brief	�R�C���̊l��
	 * @param[in]	c	�l�������R�C������
	 */
	void AddCoin(int c)
	{
		coin += c;
	}

	/**
	 * @brief	�X�R�A�̊l��
	 * @param[in]	s	�l�������X�R�A
	 */
	void AddScore(int s)
	{
		score += s;
	}

	int GetCoin() const { return coin; }
	int GetScore() const { return score; }

	/**
	 * �V���O���g���p�̃C���X�^���X�̐���
	 */
	static void Create()
	{
		assert(!instance);
		if (instance)
		{
			return;
		}
		instance = new Score();
	}

	/**
	 * �V���O���g���p�̃C���X�^���X�̔j��
	 */
	static void Release()
	{
		assert(instance);
		if (instance)
		{
			delete instance;
			return;
		}
	}

	/**
	 * �V���O���g���p�̃A�N�Z�X�|�C���g�̒�`
	 * ���̊֐����ŃC���X�^���X������A�O������Q�Ƃł���
	 */
	static Score& GetInstance()
	{
		if (instance == nullptr)
		{
			Create();
		}
		return *instance;
	}
};
//static�ϐ��̒�`
Score* Score::instance = nullptr;