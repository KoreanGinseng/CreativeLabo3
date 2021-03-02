/* Singleton�T���v��
 * Singleton...�C���X�^���X���P�������݂��Ȃ�
 * ���I�m�۔�Singleton
 *
 * ���[�J��static�ϐ��𗘗p����Singleton��
 * �C���X�^���X�̔j�����C�ӂ̃^�C�~���O�ł����Ȃ��Ȃ�
 * ���I�m�ۂȂ�K�v�Ȏ��ɐ����Ɣj�����ł���
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

void PrintUI()
{
	std::cout << "�c�@ : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
	}
	std::cout << "\n�R�C�� : " << Score::GetInstance().GetCoin() << ", �X�R�A : " << Score::GetInstance().GetScore() << "\n\n";
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

	//Singleton�C���X�^���X�̔j��
	Score::Release();
	return 0;
}