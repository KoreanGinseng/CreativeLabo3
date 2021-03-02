/* Factoryサンプル
 * Factory...オブジェクトを生成する工場
 *
 * Factoryをテンプレート化して様々なものに対応できるようにする
 * またCreatorを登録して複数の製品をkeyにより生産する形にする
 */
#include	<iostream>
#include	"Stage.h"
#include	"Factory.h"
#include	"Slime.h"
#include	"Goblin.h"

int main()
{
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