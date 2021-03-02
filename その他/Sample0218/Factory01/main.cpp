/* Factory�T���v��
 * Factory��K�p����O�̃T���v��
 *
 * �X�e�[�W���G�̐����������Ȃ�
 *
 * �G�̎�ނ������邽�тɃX�e�[�W�N���X�̏C�����K�v�ɂȂ�
 * �����̃v���O���}�ō�Ƃ𕪎U����ꍇ�͓G�̒ǉ��ŋ������N����\��������
 * �X�ɓG�̎�ނ��Ƃɓ���ȏ��������K�v�ȏꍇ�̓X�e�[�W�N���X�̕ύX���傫���Ȃ�
 */
#include	<iostream>
#include	"Stage.h"

int main()
{
	//�G�̃��X�g
	std::vector<Enemy*> enemys;
	//�X�e�[�W
	Stage stg;
	stg.Create();

	//�X�e�[�W�̃X�N���[��
	for (int scroll = 0; scroll < 30; scroll++)
	{
		//�G���o��������
		std::cout << "Scroll : " << scroll << std::endl;
		stg.Spawn(scroll, enemys);

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