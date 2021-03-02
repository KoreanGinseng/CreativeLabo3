#pragma once

#include	<iostream>

/**
 * ステータスクラス
 */
class Status
{
private:
	/** キャラクターのステータス */
	int maxHP;
	int maxMP;
	int STR;
	int DEF;
	int SPD;
public:
	/**
	 * コンストラクタ
	 */
	Status(int hp, int mp, int str, int def, int spd)
		: maxHP(hp)
		, maxMP(mp)
		, STR(str)
		, DEF(def)
		, SPD(spd)
	{
		std::cout << "ステータスが作られた!!" << std::endl;
	}
	/**
	 * デストラクタ
	 */
	~Status()
	{
		std::cout << "ステータスが破棄された!!" << std::endl;
	}
};

