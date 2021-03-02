/* Singletonサンプル
 * Singleton適用前のサンプル03
 *
 * グローバル駄目って言われた...
 * そうだstaticにしよう！！
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
//staticの実体定義
int Character::coin = 0;
int Character::score = 0;

void PrintUI()
{
	std::cout << "残機 : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
	}
	std::cout << "\nコイン : " << Character::GetCoin() << ", スコア : " << Character::GetScore() << "\n\n";
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