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

	/** コンストラクタをprivateにして外部から生成できなくする */
	/**
	 * コンストラクタ
	 */
	Score()
		: coin(0)
		, score(0)
	{
	}
	Score(const Score& obj) = delete;
	Score(Score&& obj) = delete;
public:
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

	/**
	 * シングルトン用のアクセスポイントの定義
	 * この関数内でインスタンス化され、外部から参照できる
	 */
	static Score& GetInstance()
	{
		static Score obj;
		return obj;
	}
};