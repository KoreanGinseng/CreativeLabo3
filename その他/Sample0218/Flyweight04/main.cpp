/* Flyweight&Singleton�T���v��
 * Flyweight...���\�[�X�̋��L
 * Singleton...�C���X�^���X���P�������݂��Ȃ�
 *
 * �摜��X�e�[�^�X�͓G�̏��Ƃ��ăQ�[���S�̂ň��������
 * �G���K�v�Ȏ���Singleton�ȊǗ��N���X����f�[�^���󂯎�邪�A
 * ���̎��_�ő��݂��Ȃ��ꍇ�����ǂݍ��݂␶�������s
 */
#include	<iostream>
#include	"TextureManager.h"
#include	"StatusManager.h"
#include	"Character.h"

int main()
{
	std::cout << "Initialize...\n";
	//�摜�̓X���C���ƃS�u������2���
	TextureManager textures;
	StatusManager status;
	StatusManager::GetInstance().Register("Slime", 10, 10, 5, 5, 5);
	StatusManager::GetInstance().Register("Goblin", 10, 10, 10, 0, 10);
	//�X���C����3��,�S�u������2�̏o��������
	constexpr int EnemyCount = 5;
	Character enemy[EnemyCount];

	//�ǂݍ��ݏ���
	std::cout << "\n\nNowLoading...\n";
	for (int i = 0; i < EnemyCount; i++)
	{
		enemy[i].SetTexture(i < 3 ? TextureManager::GetInstance().GetTexture("Slime.png") : TextureManager::GetInstance().GetTexture("Goblin.png"));
		enemy[i].SetStatus(i < 3 ? StatusManager::GetInstance().GetStatus("Slime") : StatusManager::GetInstance().GetStatus("Goblin"));
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