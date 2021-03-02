/* Singletonサンプル
 * Singleton...インスタンスが１つしか存在しない
 *
 * スコアとコインはゲーム中に１つのみ
 * Singletonで生成してグローバルに使えるようにアクセスできるようにして
 * キャラクターや表示に使う
 *
 * 注意：
 * 処理を見てもわかる通りグローバルとほぼ変わらない
 * グローバルとの違いはクラスのメンバーなので競合の心配がないぐらい
 * 多用するとクラス同士の結合度が高まり、
 * 各所からのアクセスで並列処理の際の問題なども出る
 * 使う前に本当に必要かよく考えよう
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
	return 0;
}