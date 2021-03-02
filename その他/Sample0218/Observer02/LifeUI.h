#pragma once

#include	"Character.h"

/**
 * 残機UIクラス
 */
class LifeUI
{
private:
	/** 残機用画像 */
	Texture texture;
public:
	/**
	 * コンストラクタ
	 */
	LifeUI()
		: texture()
	{
	}
	/**
	 * デストラクタ
	 */
	~LifeUI()
	{
	}

	/**
	 * @brief		画像の読み込み処理
	 */
	bool Load()
	{
		return texture.Load("Life.png");
	}

	/**
	 * @brief	残機を表示する
	 */
	void Print(const Character& chara) const {
		std::cout << texture.GetName() << " : " << chara.GetLife() << std::endl;
	}
};

