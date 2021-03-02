/* Observerサンプル
 * Observer...変更を監視して変更があったら通知する
 *
 * Observerを利用したリアクティブプログラミング
 * 値の変更を検出し処理をおこなう
 */
#include	<iostream>
#include	"Character.h"
#include	"ScoreUI.h"
#include	"CoinUI.h"
#include	"LifeUI.h"

int main()
{
	//ゲーム処理
	Character player;
	ScoreUI score;
	CoinUI coin;
	LifeUI life;
	player.Load();
	player.GetScoreSubject()->Subscribe(&score);
	player.GetCoinSubject()->Subscribe(&coin);
	player.GetLifeSubject()->Subscribe(&life);
	score.Load();
	coin.Load();
	life.Load();

	score.Print();
	coin.Print();
	life.Print();

	player.AcquiredCoin();
	score.Print();
	coin.Print();
	life.Print();

	player.KillEnemy();
	score.Print();
	coin.Print();
	life.Print();

	player.Dead();
	score.Print();
	coin.Print();
	life.Print();

	return 0;
}