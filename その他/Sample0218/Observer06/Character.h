#pragma once

#include	"Texture.h"
#include	"ReactiveParameter.h"

/**
 * キャラクタークラス
 */
class Character
{
private:
	/** キャラクターの残機 */
	ReactiveParameter<int> life;
	/** キャラクターのコイン */
	ReactiveParameter<int> coin;
	/** キャラクターのスコア */
	ReactiveParameter<int> score;

	/** キャラクターは画像を持つ */
	Texture texture;
public:
	/**
	 * コンストラクタ
	 */
	Character()
		: life(3)
		, coin(0)
		, score(0)
		, texture()
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
		return texture.Load("Player.png");
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

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
	IObservable<int>* GetLifeSubject() { return &life; }
	IObservable<int>* GetCoinSubject() { return &coin; }
	IObservable<int>* GetScoreSubject() { return &score; }
};

