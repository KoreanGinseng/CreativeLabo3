#pragma once

#include	"Texture.h"

/**
 * キャラクタークラス
 */
class Character
{
private:
	/** キャラクターは画像を参照する */
	Texture* texture;

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
		: texture(nullptr)
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
	 * @brief		画像の設定
	 */
	void SetTexture(Texture* tex)
	{
		std::cout << "キャラクタに利用する画像を設定" << std::endl;
		texture = tex;
	}

	/**
	 * @brief		敵の表示
	 */
	void Render()
	{
		std::cout << "画像[" << texture->GetName() << "]を表示" << std::endl;
	}
};

