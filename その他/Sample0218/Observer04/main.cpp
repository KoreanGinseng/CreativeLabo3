/* Observer�T���v��
 * Observer...�ύX���Ď����ĕύX����������ʒm����
 *
 * Character���ɒʒm�p��Subject���쐬���A
 * UI����ʒm���󂯂�Observer�Ƃ��č쐬����B
 * �ύX���������^�C�~���O��Character����ʒm�𔭍s����
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