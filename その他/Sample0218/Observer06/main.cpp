/* Observer�T���v��
 * Observer...�ύX���Ď����ĕύX����������ʒm����
 *
 * Observer�𗘗p�������A�N�e�B�u�v���O���~���O
 * �l�̕ύX�����o�������������Ȃ�
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
	player.GetScoreSubject()->Subscribe(&score);
	player.GetCoinSubject()->Subscribe(&coin);
	player.GetLifeSubject()->Subscribe(&life);
	score.Load();
	coin.Load();
	life.Load();

	score.Print();
	coin.Print();
	life.Print();

	player.AcquiredCoin();
	score.Print();
	coin.Print();
	life.Print();

	player.KillEnemy();
	score.Print();
	coin.Print();
	life.Print();

	player.Dead();
	score.Print();
	coin.Print();
	life.Print();

	return 0;
}