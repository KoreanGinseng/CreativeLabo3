#pragma once

#include	"Texture.h"

/**
 * キャラクタークラス
 */
class Character
{
private:
	/** キャラクターは画像を持つ */
	Texture texture;

	/** キャラクターのステータス */
	int maxHP;
	int maxMP;
	int STR;
	int DEF;
	int SPD;

	/** キャラクターの位置 */
	int posX;
	int posY;
public:
	/**
	 * コンストラクタ
	 */
	Character()
		: texture()
		, maxHP(10)
		, maxMP(10)
		, STR(5)
		, DEF(5)
		, SPD(5)
		, posX(0)
		, posY(0)
	{
		std::cout << "キャラクタが作られた!!" << std::endl;
	}
	/**
	 * デストラクタ
	 */
	~Character()
	{
		std::cout << "キャラクタが破棄された!!" << std::endl;
	}

	/**
	 * @brief		画像の読み込み処理
	 * @param[in]	fname		ファイル名
	 */
	bool Load(const std::string& fname)
	{
		return texture.Load(fname);
	}

	/**
	 * @brief		敵の表示
	 */
	void Render()
	{
		std::cout << "画像[" << texture.GetName() << "]を表示" << std::endl;
	}
};

