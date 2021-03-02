/* Singleton�T���v��
 * Singleton�K�p�O�̃T���v��01
 *
 * �v���C���[���ꂼ�ꂪ�ʂɕێ����ĕ\�����ɑS�����Z���ĕ\��
 *
 * ��:
 * New�}���I�u���U�[�Y�̂悤�ȃQ�[�����l����
 * �v���C���[�͍ő�4�l�܂œ����v���C
 * �c�@�̓v���C���[���ƂɌʂŕێ�
 * �R�C���̎擾������X�R�A�̓v���C���[�S���ŋ���
 * �����ǂ��������������悤�H
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
	//�R�C���ƃX�R�A�̍��v�v�Z�p
	int coin = 0;
	int score = 0;
	std::cout << "�c�@ : ";
	for (int i = 0; i < PlayerCount; i++)
	{
		std::cout << "[" << i << "] : " << player[i].GetLife() << ", ";
		coin += player[i].GetCoin();
		score += player[i].GetScore();
	}
	std::cout << "\n�R�C�� : " << coin << ", �X�R�A : " << score << "\n\n";
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