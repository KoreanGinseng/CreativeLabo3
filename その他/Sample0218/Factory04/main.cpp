/* Factory�T���v��
 * Flyweight...���\�[�X�̋��L
 * Singleton...�C���X�^���X���P�������݂��Ȃ�
 * Factory...�I�u�W�F�N�g�𐶐�����H��
 *
 * �����̓��e�S���g���Ă݂��T���v��
 */
#include	<iostream>
#include	"Stage.h"
#include	"Factory.h"
#include	"TextureManager.h"
#include	"StatusManager.h"
#include	"Slime.h"
#include	"Goblin.h"

int main()
{
	//�e�N�X�`���ƃX�e�[�^�X�Ǘ�
	TextureManager textures;
	StatusManager status;
	StatusManager::GetInstance().Register("Slime", 10, 10, 5, 5, 5);
	StatusManager::GetInstance().Register("Goblin", 10, 10, 10, 0, 10);
	//�G�̃��X�g
	std::vector<Enemy*> enemys;
	//�G�̐������X�g
	Factory<Enemy> enemyFactorys;
	enemyFactorys.Register<Slime>("Slime");
	enemyFactorys.Register<Goblin>("Goblin");
	//�X�e�[�W
	Stage stg;
	stg.Create();

	//�X�e�[�W�̃X�N���[��
	for (int scroll = 0; scroll < 30; scroll++)
	{
		//�G���o��������
		std::cout << "Scroll : " << scroll << std::endl;
		stg.Spawn(scroll, enemyFactorys, enemys);
		for (auto en : enemys)
		{
			en->Render();
		}

		//�X�N���[��
		scroll++;
		std::cout << std::endl;
	}

	//�S�����
	while (enemys.size() > 0)
	{
		delete enemys[enemys.size() - 1];
		enemys.pop_back();
	}
	return 0;
}