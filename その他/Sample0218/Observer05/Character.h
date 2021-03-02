#pragma once

#include	"Texture.h"
#include	"Subject.h"

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

	/** 残機変更通知 */
	Subject<int> lifeSubject;
	/** コイン変更通知 */
	Subject<int> coinSubject;
	/** スコア変更通知 */
	Subject<int> scoreSubject;
public:
	/**
	 * コンストラクタ
	 */
	Character()
		: life(3)
		, coin(0)
		, score(0)
		, texture()
		, lifeSubject()
		, coinSubject()
		, scoreSubject()
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
		lifeSubject.Notify(life);
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
		coinSubject.Notify(coin);
		scoreSubject.Notify(score);
		std::cout << "プレイヤーがコインを獲得" << std::endl;
	}

	/**
	 * @brief	敵を倒した
	 *			スコアを200増やす
	 */
	void KillEnemy()
	{
		score += 200;
		scoreSubject.Notify(score);
		std::cout << "プレイヤーが敵を倒した" << std::endl;
	}

	int GetLife() const { return life; }
	int GetCoin() const { return coin; }
	int GetScore() const { return score; }
	IObservable<int>* GetLifeSubject() { return &lifeSubject; }
	IObservable<int>* GetCoinSubject() { return &coinSubject; }
	IObservable<int>* GetScoreSubject() { return &scoreSubject; }
};

