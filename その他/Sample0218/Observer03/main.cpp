/* Observerサンプル
 * Observer適用前のサンプル03
 *
 * UIにキャラクターを引数は結合度が高くて良くないと言われた・・・
 * そうだスコアやHPにも変数を持たせてSetする形にしよう！！
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
	score.Load();
	coin.Load();
	life.Load();

	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	player.AcquiredCoin();
	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	player.KillEnemy();
	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	player.Dead();
	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	return 0;
}