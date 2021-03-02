/* Singleton�T���v��
 * Singleton�K�p�O�̃T���v��04
 *
 * static���ǂ��Ȃ����Č���ꂽ...
 * �������X�R�A�ƃR�C�������ʃN���X�ɂ���΂����񂾁I�I
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
Score score;

void PrintUI()
{
	std::cout << "�c�@ : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
	}
	std::cout << "\n�R�C�� : " << score.GetCoin() << ", �X�R�A : " << score.GetScore() << "\n\n";
}

int main()
{
	//�Q�[������
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