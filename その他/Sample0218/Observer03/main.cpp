/* Observer�T���v��
 * Observer�K�p�O�̃T���v��03
 *
 * UI�ɃL�����N�^�[�������͌����x�������ėǂ��Ȃ��ƌ���ꂽ�E�E�E
 * �������X�R�A��HP�ɂ��ϐ�����������Set����`�ɂ��悤�I�I
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

	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	player.AcquiredCoin();
	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	player.KillEnemy();
	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	player.Dead();
	score.SetScore(player.GetScore());
	coin.SetCoin(player.GetCoin());
	life.SetLife(player.GetLife());
	score.Print();
	coin.Print();
	life.Print();

	return 0;
}