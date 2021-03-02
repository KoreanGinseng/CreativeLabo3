/* Observer�T���v��
 * Observer...�ύX���Ď����ĕύX����������ʒm����
 *
 * �ʒm���󂯂鑤���֐��݂̂œo�^�ł���悤�ɍ쐬
 * �p�����s�v�Ń����_���Œʒm�̎󂯎�肪�L�q�ł���
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
	auto obScore = player.GetScoreSubject()->Subscribe([&](int val) {score.SetScore(val); });
	auto obCoin = player.GetCoinSubject()->Subscribe([&](int val) {coin.SetCoin(val); });
	auto obLife = player.GetLifeSubject()->Subscribe([&](int val) {life.SetLife(val); });
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

	delete obScore;
	delete obCoin;
	delete obLife;
	return 0;
}