/* Observerサンプル
 * Observer...変更を監視して変更があったら通知する
 *
 * 通知を受ける側を関数のみで登録できるように作成
 * 継承が不要でラムダ式で通知の受け取りが記述できる
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
	auto obScore = player.GetScoreSubject()->Subscribe([&](int val) {score.SetScore(val); });
	auto obCoin = player.GetCoinSubject()->Subscribe([&](int val) {coin.SetCoin(val); });
	auto obLife = player.GetLifeSubject()->Subscribe([&](int val) {life.SetLife(val); });
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

	delete obScore;
	delete obCoin;
	delete obLife;
	return 0;
}