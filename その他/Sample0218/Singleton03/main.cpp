/* Singleton�T���v��
 * Singleton�K�p�O�̃T���v��03
 *
 * �O���[�o���ʖڂ��Č���ꂽ...
 * ������static�ɂ��悤�I�I
 */
#include	<iostream>
#include	"Character.h"

 //�v���C���[��4�l
constexpr int PlayerCount = 4;
Character player[PlayerCount] = {
	Character(0),
	Character(1),
	Character(2),
	Character(3),
};
//static�̎��̒�`
int Character::coin = 0;
int Character::score = 0;

void PrintUI()
{
	std::cout << "�c�@ : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
	}
	std::cout << "\n�R�C�� : " << Character::GetCoin() << ", �X�R�A : " << Character::GetScore() << "\n\n";
}

int main()
{
	//�Q�[������
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