/* Flyweight�T���v��
 * Flyweight��K�p����O�̃T���v��
 *
 * �Q�[���Ȃ�G�͓����摜�̂��̂������o�ꂷ��
 * �ʂɂ��̂܂܍��Ɠo�ꂷ��G�̐������摜������A�ǂݍ��܂��...
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	//����������
	std::cout << "Initialize...\n";
	//�X���C����3��,�S�u������2�̏o��������
	constexpr int EnemyCount = 5;
	Character enemy[EnemyCount];

	//�ǂݍ��ݏ���
	std::cout << "\n\nNowLoading...\n";
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].Load(i < 3 ? "Slime.png" : "Goblin.png");
	}

	//�Q�[������
	std::cout << "\n\nBattle...\n";
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].Render();
	}

	//�I���
	std::cout << "\n\nBattleEnd...\n";
	return 0;
}