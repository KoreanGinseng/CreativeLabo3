/* Singletonサンプル
 * Singleton適用前のサンプル01
 *
 * プレイヤーそれぞれが個別に保持して表示時に全部加算して表示
 *
 * 例:
 * Newマリオブラザーズのようなゲームを考える
 * プレイヤーは最大4人まで同時プレイ
 * 残機はプレイヤーごとに個別で保持
 * コインの取得枚数やスコアはプレイヤー全員で共通
 * さぁどういう実装をしよう？
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
	//コインとスコアの合計計算用
	int coin = 0;
	int score = 0;
	std::cout << "残機 : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
		coin += player[i].GetCoin();
		score += player[i].GetScore();
	}
	std::cout << "\nコイン : " << coin << ", スコア : " << score << "\n\n";
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