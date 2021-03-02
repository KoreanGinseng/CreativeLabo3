/* Observer�T���v��
 * Observer�K�p�O�̃T���v��02
 *
 * �X�R�A��HP�Ȃǂ�UI�𕪗�
 */
#include	<iostream>
#include	"Character.h"
#include	"ScoreUI.h"
#include	"CoinUI.h"
#include	"LifeUI.h"

int main()
{
	//�Q�[������
	Character player;
	ScoreUI score;
	CoinUI coin;
	LifeUI life;
	player.Load();
	score.Load();
	coin.Load();
	life.Load();

	score.Print(player);
	coin.Print(player);
	life.Print(player);

	player.AcquiredCoin();
	score.Print(player);
	coin.Print(player);
	life.Print(player);

	player.KillEnemy();
	score.Print(player);
	coin.Print(player);
	life.Print(player);

	player.Dead();
	score.Print(player);
	coin.Print(player);
	life.Print(player);

	return 0;
}