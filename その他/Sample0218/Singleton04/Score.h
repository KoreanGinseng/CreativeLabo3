#pragma once

#include	<iostream>

/**
 * スコアクラス
 */
class Score
{
private:
	/** コイン */
	int coin;
	/** スコア */
	int score;
public:
	/**
	 * コンストラクタ
	 */
	Score()
		: coin(0)
		, score(0)
	{
	}
	/**
	 * デストラクタ
	 */
	~Score()
	{
	}

	/**
	 * @brief	コインの獲得
	 * @param[in]	c	獲得したコイン枚数
	 */
	void AddCoin(int c)
	{
		coin += c;
	}

	/**
	 * @brief	スコアの獲得
	 * @param[in]	s	獲得したスコア
	 */
	void AddScore(int s)
	{
		score += s;
	}

	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
};