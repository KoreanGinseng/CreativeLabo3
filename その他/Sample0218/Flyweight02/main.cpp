/* Flyweightサンプル
 * Flyweight...リソースの共有
 *
 * テクスチャをポインタ参照で共有させたパターン
 * 画像の生成と読み込みは種類の数だけで済む
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	std::cout << "Initialize...\n";
	//画像はスライムとゴブリンの2種類
	constexpr int EnemyType = 2;
	Texture texture[EnemyType];
	//スライムを3体,ゴブリンを2体出現させる
	constexpr int EnemyCount = 5;
	Character enemy[EnemyCount];

	//読み込み処理
	std::cout << "\n\nNowLoading...\n";
	texture[0].Load("Slime.png");
	texture[1].Load("Goblin.png");
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].SetTexture(i < 3 ? &texture[0] : &texture[1]);
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