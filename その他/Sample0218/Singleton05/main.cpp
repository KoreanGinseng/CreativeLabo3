/* Singleton�T���v��
 * Singleton...�C���X�^���X���P�������݂��Ȃ�
 *
 * �X�R�A�ƃR�C���̓Q�[�����ɂP�̂�
 * Singleton�Ő������ăO���[�o���Ɏg����悤�ɃA�N�Z�X�ł���悤�ɂ���
 * �L�����N�^�[��\���Ɏg��
 *
 * ���ӁF
 * ���������Ă��킩��ʂ�O���[�o���Ƃقڕς��Ȃ�
 * �O���[�o���Ƃ̈Ⴂ�̓N���X�̃����o�[�Ȃ̂ŋ����̐S�z���Ȃ����炢
 * ���p����ƃN���X���m�̌����x�����܂�A
 * �e������̃A�N�Z�X�ŕ��񏈗��̍ۂ̖��Ȃǂ��o��
 * �g���O�ɖ{���ɕK�v���悭�l���悤
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
	return 0;
}