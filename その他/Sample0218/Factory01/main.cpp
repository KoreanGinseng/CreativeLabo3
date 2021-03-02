/* Factoryサンプル
 * Factoryを適用する前のサンプル
 *
 * ステージが敵の生成をおこなう
 *
 * 敵の種類が増えるたびにステージクラスの修正が必要になり
 * 複数のプログラマで作業を分散する場合は敵の追加で競合が起きる可能性がある
 * 更に敵の種類ごとに特殊な初期化が必要な場合はステージクラスの変更が大きくなる
 */
#include	<iostream>
#include	"Stage.h"

int main()
{
	//敵のリスト
	std::vector<Enemy*> enemys;
	//ステージ
	Stage stg;
	stg.Create();

	//ステージのスクロール
	for (int scroll = 0; scroll < 30; scroll++)
	{
		//敵を出現させる
		std::cout << "Scroll : " << scroll << std::endl;
		stg.Spawn(scroll, enemys);

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