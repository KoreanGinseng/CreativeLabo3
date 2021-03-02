/* Observerサンプル
 * Observer適用前のサンプル02
 *
 * スコアやHPなどのUIを分離
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

	score.Print(player);
	coin.Print(player);
	life.Print(player);

	player.AcquiredCoin();
	score.Print(player);
	coin.Print(player);
	life.Print(player);

	player.KillEnemy();
	score.Print(player);
	coin.Print(player);
	life.Print(player);

	player.Dead();
	score.Print(player);
	coin.Print(player);
	life.Print(player);

	return 0;
}