/* Flyweight�T���v��
 * Flyweight...���\�[�X�̋��L
 *
 * �e�N�X�`�����|�C���^�Q�Ƃŋ��L�������p�^�[��
 * �摜�̐����Ɠǂݍ��݂͎�ނ̐������ōς�
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	std::cout << "Initialize...\n";
	//�摜�̓X���C���ƃS�u������2���
	constexpr int EnemyType = 2;
	Texture texture[EnemyType];
	//�X���C����3��,�S�u������2�̏o��������
	constexpr int EnemyCount = 5;
	Character enemy[EnemyCount];

	//�ǂݍ��ݏ���
	std::cout << "\n\nNowLoading...\n";
	texture[0].Load("Slime.png");
	texture[1].Load("Goblin.png");
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].SetTexture(i < 3 ? &texture[0] : &texture[1]);
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