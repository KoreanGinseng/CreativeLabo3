/* Singletonサンプル
 * Singleton...インスタンスが１つしか存在しない
 * 動的確保版Singleton
 *
 * ローカルstatic変数を利用したSingletonは
 * インスタンスの破棄が任意のタイミングでおこなえない
 * 動的確保なら必要な時に生成と破棄ができる
 */
#include	<iostream>
#include	"Character.h"

 //プレイヤーは4人
constexpr int PlayerCount = 4;
Character player[PlayerCount] = {
	Character(0),
	Character(1),
	Character(2),
	Character(3),
};

void PrintUI()
{
	std::cout << "残機 : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
	}
	std::cout << "\nコイン : " << Score::GetInstance().GetCoin() << ", スコア : " << Score::GetInstance().GetScore() << "\n\n";
}

int main()
{
	//ゲーム処理
	player[0].AcquiredCoin();
	PrintUI();
	player[0].AcquiredCoin();
	PrintUI();
	player[1].KillEnemy();
	PrintUI();
	player[2].AcquiredCoin();
	PrintUI();
	player[1].KillEnemy();
	PrintUI();
	player[3].Dead();
	PrintUI();
	player[3].Dead();
	PrintUI();
	player[1].Dead();
	PrintUI();
	player[0].KillEnemy();
	PrintUI();

	//Singletonインスタンスの破棄
	Score::Release();
	return 0;
}