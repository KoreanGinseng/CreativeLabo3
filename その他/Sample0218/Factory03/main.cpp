/* Factory�T���v��
 * Factory...�I�u�W�F�N�g�𐶐�����H��
 *
 * Factory���e���v���[�g�����ėl�X�Ȃ��̂ɑΉ��ł���悤�ɂ���
 * �܂�Creator��o�^���ĕ����̐��i��key�ɂ�萶�Y����`�ɂ���
 */
#include	<iostream>
#include	"Stage.h"
#include	"Factory.h"
#include	"Slime.h"
#include	"Goblin.h"

int main()
{
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