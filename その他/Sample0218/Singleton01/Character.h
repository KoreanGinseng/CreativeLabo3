#pragma once

#include	<iostream>

/**
 * キャラクタークラス
 */
class Character
{
private:
	/** キャラクターID */
	int id;
	/** キャラクターの残機 */
	int life;
	/** キャラクターのコイン */
	int coin;
	/** キャラクターのスコア */
	int score;
public:
	/**
	 * コンストラクタ
	 */
	Character(int id)
		: id(id)
		, life(3)
		, coin(0)
		, score(0)
	{
	}
	/**
	 * デストラクタ
	 */
	~Character()
	{
	}

	/**
	 * @brief	死亡
	 *			残機を減らす
	 */
	void Dead()
	{
		life -= 1;
		std::cout << "プレイヤー[" << id << "]が死んでしまった" << std::endl;
	}

	/**
	 * @brief	コインの獲得
	 *			コインを1枚増やして、スコアを100増やす
	 */
	void AcquiredCoin()
	{
		coin += 1;
		score += 100;
		std::cout << "プレイヤー[" << id << "]がコインを獲得" << std::endl;
	}

	/**
	 * @brief	敵を倒した
	 *			スコアを200増やす
	 */
	void KillEnemy()
	{
		score += 200;
		std::cout << "プレイヤー[" << id << "]が敵を倒した" << std::endl;
	}

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
};

