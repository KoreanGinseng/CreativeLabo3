#pragma once

#include	<iostream>

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
public:
	/**
	 * �R���X�g���N�^
	 */
	Score()
		: coin(0)
		, score(0)
	{
	}
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
};