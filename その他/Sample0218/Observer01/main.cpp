/* Observerサンプル
 * Observer適用前のサンプル01
 *
 * プレイヤーがHPやスコアといったUIを表示
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	//ゲーム処理
	Character player;
	player.Load();

	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	player.AcquiredCoin();
	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	player.KillEnemy();
	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	player.Dead();
	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	return 0;
}