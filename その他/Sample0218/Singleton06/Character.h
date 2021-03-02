#pragma once

#include	<iostream>
#include	"Score.h"

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
		Score::GetInstance().AddCoin(1);
		Score::GetInstance().AddScore(100);
		std::cout << "プレイヤー[" << id << "]がコインを獲得" << std::endl;
	}

	/**
	 * @brief	敵を倒した
	 *			スコアを200増やす
	 */
	void KillEnemy()
	{
		Score::GetInstance().AddScore(200);
		std::cout << "プレイヤー[" << id << "]が敵を倒した" << std::endl;
	}

	int GetLife() const { return life; }
};

