/* Flyweight&Singletonサンプル
 * Flyweight...リソースの共有
 * Singleton...インスタンスが１つしか存在しない
 *
 * 画像やステータスは敵の情報としてゲーム全体で一つだけ生成
 * 敵が必要な時にSingletonな管理クラスからデータを受け取るが、
 * その時点で存在しない場合だけ読み込みや生成を実行
 */
#include	<iostream>
#include	"TextureManager.h"
#include	"StatusManager.h"
#include	"Character.h"

int main()
{
	std::cout << "Initialize...\n";
	//画像はスライムとゴブリンの2種類
	TextureManager textures;
	StatusManager status;
	StatusManager::GetInstance().Register("Slime", 10, 10, 5, 5, 5);
	StatusManager::GetInstance().Register("Goblin", 10, 10, 10, 0, 10);
	//スライムを3体,ゴブリンを2体出現させる
	constexpr int EnemyCount = 5;
	Character enemy[EnemyCount];

	//読み込み処理
	std::cout << "\n\nNowLoading...\n";
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].SetTexture(i < 3 ? TextureManager::GetInstance().GetTexture("Slime.png") : TextureManager::GetInstance().GetTexture("Goblin.png"));
		enemy[i].SetStatus(i < 3 ? StatusManager::GetInstance().GetStatus("Slime") : StatusManager::GetInstance().GetStatus("Goblin"));
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