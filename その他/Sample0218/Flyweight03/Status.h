#pragma once

#include	<iostream>

/**
 * �X�e�[�^�X�N���X
 */
class Status
{
private:
	/** �L�����N�^�[�̃X�e�[�^�X */
	int maxHP;
	int maxMP;
	int STR;
	int DEF;
	int SPD;
public:
	/**
	 * �R���X�g���N�^
	 */
	Status(int hp, int mp, int str, int def, int spd)
		: maxHP(hp)
		, maxMP(mp)
		, STR(str)
		, DEF(def)
		, SPD(spd)
	{
		std::cout << "�X�e�[�^�X�����ꂽ!!" << std::endl;
	}
	/**
	 * �f�X�g���N�^
	 */
	~Status()
	{
		std::cout << "�X�e�[�^�X���j�����ꂽ!!" << std::endl;
	}
};

