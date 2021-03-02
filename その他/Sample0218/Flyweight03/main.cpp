/* Flyweight�T���v��
 * Flyweight...���\�[�X�̋��L
 *
 * �X�e�[�^�X�����L�������p�^�[��
 * �G�̎�ނ��ƂɃp�����[�^�[�����ʂŕω����Ȃ��Ȃ�X�e�[�^�X�����L�ł���
 *
 * Unity�ł�ScriptableObject���g���Ε����̃I�u�W�F�N�g��
 * �p�����[�^�[�����L�����邱�Ƃ��ł���
 *
 * �X�e�[�^�X�Ȃǂ��I�u�W�F�N�g�������o������Ȃ�
 * �y���ʂ͂���Ȃ�ɂȂ�
 * �X�ɃI�u�W�F�N�g�̃��������p�ʂ����邱�ƂŐ����������ɂȂ�
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	std::cout << "Initialize...\n";
	//�摜�̓X���C���ƃS�u������2���
	constexpr int EnemyType = 2;
	Texture texture[EnemyType];
	Status status[EnemyType] = {
		Status(10,10,5,5,5),	//�X���C��
		Status(10,10,10,0,10)	//�S�u����
	};
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
		enemy[i].SetStatus(i < 3 ? &status[0] : &status[1]);
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