#pragma once

#include	<iostream>

//グローバルの変数を使えるように外部参照
extern int coin;
extern int score;

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
public:
	/**
	 * コンストラクタ
	 */
	Character(int id)
		: id(id)
		, life(3)
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
};

