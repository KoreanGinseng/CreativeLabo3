/* Factory�T���v��
 * Factory...�I�u�W�F�N�g�𐶐�����H��
 *
 * �X�e�[�W��Factory�������Ȃ�
 * �X�e�[�W�͏o���ʒu�Ƃǂ̎�ނ̓G����������邩�������Ă���
 * Factory�ɐ�������Product��o�^���Ă����A������C����
 */
#include	<iostream>
#include	"Stage.h"
#include	"SlimeFactory.h"
#include	"GoblinFactory.h"

int main()
{
	//�G�̃��X�g
	std::vector<Enemy*> enemys;
	//�G�̐������X�g
	std::vector< EnemyFactory* > enemyFactorys{
		new SlimeFactory(),
		new GoblinFactory()
	};
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
	while (enemyFactorys.size() > 0)
	{
		delete enemyFactorys[enemyFactorys.size() - 1];
		enemyFactorys.pop_back();
	}
	return 0;
}