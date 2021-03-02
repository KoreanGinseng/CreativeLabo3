#pragma once

#include	"Texture.h"

/**
 * キャラクタークラス
 */
class Character
{
private:
	/** キャラクターの残機 */
	int life;
	/** キャラクターのコイン */
	int coin;
	/** キャラクターのスコア */
	int score;

	/** キャラクターは画像を持つ */
	Texture texture;
	/** スコア用画像 */
	Texture scoreTexture;
	/** コイン用画像 */
	Texture coinTexture;
	/** ライフ用画像 */
	Texture lifeTexture;
public:
	/**
	 * コンストラクタ
	 */
	Character()
		: life(3)
		, coin(0)
		, score(0)
		, texture()
		, scoreTexture()
		, coinTexture()
		, lifeTexture()
	{
	}
	/**
	 * デストラクタ
	 */
	~Character()
	{
	}

	/**
	 * @brief		画像の読み込み処理
	 */
	bool Load()
	{
		if (!texture.Load("Player.png")) { return false; }
		if (!scoreTexture.Load("Score.png")) { return false; }
		if (!coinTexture.Load("Coin.png")) { return false; }
		if (!lifeTexture.Load("Life.png")) { return false; }
		return true;
	}

	/**
	 * @brief	死亡
	 *			残機を減らす
	 */
	void Dead()
	{
		life -= 1;
		std::cout << "プレイヤーが死んでしまった" << std::endl;
	}

	/**
	 * @brief	コインの獲得
	 *			コインを1枚増やして、スコアを100増やす
	 */
	void AcquiredCoin()
	{
		coin += 1;
		score += 100;
		std::cout << "プレイヤーがコインを獲得" << std::endl;
	}

	/**
	 * @brief	敵を倒した
	 *			スコアを200増やす
	 */
	void KillEnemy()
	{
		score += 200;
		std::cout << "プレイヤーが敵を倒した" << std::endl;
	}

	/**
	 * @brief	スコアを表示する
	 */
	void PrintScore() const {
		std::cout << scoreTexture.GetName() << " : " << score << std::endl;
	}
	/**
	 * @brief	コインを表示する
	 */
	void PrintCoin() const {
		std::cout << coinTexture.GetName() << " : " << coin << std::endl;
	}
	/**
	 * @brief	ライフを表示する
	 */
	void PrintHP() const {
		std::cout << lifeTexture.GetName() << " : " << life << std::endl;
	}

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
};

