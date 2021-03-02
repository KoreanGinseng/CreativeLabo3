/* Singletonサンプル
 * Singleton...インスタンスが１つしか存在しない
 * 
 * Game Programming Gemsで紹介されていた
 * インスタンスの生成と破棄は別で管理し、
 * インスタンスの複数生成防止とアクセスポイントだけ提供するパターン
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
	//ゲーム処理中だけスコアは有効
	Score score;
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
	return 0;
}