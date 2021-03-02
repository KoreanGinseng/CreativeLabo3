/* Observerサンプル
 * Observer...変更を監視して変更があったら通知する
 *
 * Character側に通知用のSubjectを作成し、
 * UI側を通知を受けるObserverとして作成する。
 * 変更があったタイミングでCharacterから通知を発行する
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