/* Flyweightサンプル
 * Flyweightを適用する前のサンプル
 *
 * ゲームなら敵は同じ画像のものが複数登場する
 * 個別にそのまま作ると登場する敵の数だけ画像が作られ、読み込まれる...
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	//初期化処理
	std::cout << "Initialize...\n";
	//スライムを3体,ゴブリンを2体出現させる
	constexpr int EnemyCount = 5;
	Character enemy[EnemyCount];

	//読み込み処理
	std::cout << "\n\nNowLoading...\n";
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].Load(i < 3 ? "Slime.png" : "Goblin.png");
	}

	//ゲーム処理
	std::cout << "\n\nBattle...\n";
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].Render();
	}

	//終わり
	std::cout << "\n\nBattleEnd...\n";
	return 0;
}