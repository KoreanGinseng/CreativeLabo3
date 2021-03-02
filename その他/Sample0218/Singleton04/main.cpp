/* Singletonサンプル
 * Singleton適用前のサンプル04
 *
 * staticも良くないって言われた...
 * そうだスコアとコインだけ別クラスにすればいいんだ！！
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
Score score;

void PrintUI()
{
	std::cout << "残機 : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
	}
	std::cout << "\nコイン : " << score.GetCoin() << ", スコア : " << score.GetScore() << "\n\n";
}

int main()
{
	//ゲーム処理
	player[0].AcquiredCoin(score);
	PrintUI();
	player[0].AcquiredCoin(score);
	PrintUI();
	player[1].KillEnemy(score);
	PrintUI();
	player[2].AcquiredCoin(score);
	PrintUI();
	player[1].KillEnemy(score);
	PrintUI();
	player[3].Dead();
	PrintUI();
	player[3].Dead();
	PrintUI();
	player[1].Dead();
	PrintUI();
	player[0].KillEnemy(score);
	PrintUI();
	return 0;
}