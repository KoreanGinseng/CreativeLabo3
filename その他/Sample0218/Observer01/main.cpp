/* Observer�T���v��
 * Observer�K�p�O�̃T���v��01
 *
 * �v���C���[��HP��X�R�A�Ƃ�����UI��\��
 */
#include	<iostream>
#include	"Character.h"

int main()
{
	//�Q�[������
	Character player;
	player.Load();

	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	player.AcquiredCoin();
	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	player.KillEnemy();
	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	player.Dead();
	player.PrintScore();
	player.PrintCoin();
	player.PrintHP();

	return 0;
}