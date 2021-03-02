/* Factoryサンプル
 * Factory...オブジェクトを生成する工場
 *
 * ステージはFactoryがおこなう
 * ステージは出現位置とどの種類の敵が生成されるかを持っている
 * Factoryに生成するProductを登録しておき、生成を任せる
 */
#include	<iostream>
#include	"Stage.h"
#include	"SlimeFactory.h"
#include	"GoblinFactory.h"

int main()
{
	//敵のリスト
	std::vector<Enemy*> enemys;
	//敵の生成リスト
	std::vector< EnemyFactory* > enemyFactorys{
		new SlimeFactory(),
		new GoblinFactory()
	};
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
	while (enemyFactorys.size() > 0)
	{
		delete enemyFactorys[enemyFactorys.size() - 1];
		enemyFactorys.pop_back();
	}
	return 0;
}