/* Factoryサンプル
 * Flyweight...リソースの共有
 * Singleton...インスタンスが１つしか存在しない
 * Factory...オブジェクトを生成する工場
 *
 * 今日の内容全部使ってみたサンプル
 */
#include	<iostream>
#include	"Stage.h"
#include	"Factory.h"
#include	"TextureManager.h"
#include	"StatusManager.h"
#include	"Slime.h"
#include	"Goblin.h"

int main()
{
	//テクスチャとステータス管理
	TextureManager textures;
	StatusManager status;
	StatusManager::GetInstance().Register("Slime", 10, 10, 5, 5, 5);
	StatusManager::GetInstance().Register("Goblin", 10, 10, 10, 0, 10);
	//敵のリスト
	std::vector<Enemy*> enemys;
	//敵の生成リスト
	Factory<Enemy> enemyFactorys;
	enemyFactorys.Register<Slime>("Slime");
	enemyFactorys.Register<Goblin>("Goblin");
	//ステージ
	Stage stg;
	stg.Create();

	//ステージのスクロール
	for (int scroll = 0; scroll < 30; scroll++)
	{
		//敵を出現させる
		std::cout << "Scroll : " << scroll << std::endl;
		stg.Spawn(scroll, enemyFactorys, enemys);
		for (auto en : enemys)
		{
			en->Render();
		}

		//スクロール
		scroll++;
		std::cout << std::endl;
	}

	//全部解放
	while (enemys.size() > 0)
	{
		delete enemys[enemys.size() - 1];
		enemys.pop_back();
	}
	return 0;
}