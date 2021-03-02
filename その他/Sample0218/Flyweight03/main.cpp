/* Flyweightサンプル
 * Flyweight...リソースの共有
 *
 * ステータスも共有させたパターン
 * 敵の種類ごとにパラメーターが共通で変化しないならステータスも共有できる
 *
 * UnityではScriptableObjectを使えば複数のオブジェクトで
 * パラメーターを共有させることができる
 *
 * ステータスなどもオブジェクトが複数出現するなら
 * 軽減量はそれなりになる
 * 更にオブジェクトのメモリ利用量が減ることで生成も高速になる
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	std::cout << "Initialize...\n";
	//画像はスライムとゴブリンの2種類
	constexpr int EnemyType = 2;
	Texture texture[EnemyType];
	Status status[EnemyType] = {
		Status(10,10,5,5,5),	//スライム
		Status(10,10,10,0,10)	//ゴブリン
	};
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
		enemy[i].SetStatus(i < 3 ? &status[0] : &status[1]);
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