#pragma once

#include	"Character.h"

/**
 * コインUIクラス
 */
class CoinUI
{
private:
	/** コイン用画像 */
	Texture texture;
public:
	/**
	 * コンストラクタ
	 */
	CoinUI()
		: texture()
	{
	}
	/**
	 * デストラクタ
	 */
	~CoinUI()
	{
	}

	/**
	 * @brief		画像の読み込み処理
	 */
	bool Load()
	{
		return texture.Load("Coin.png");
	}

	/**
	 * @brief	コインを表示する
	 */
	void Print(const Character& chara) const {
		std::cout << texture.GetName() << " : " << chara.GetCoin() << std::endl;
	}
};

