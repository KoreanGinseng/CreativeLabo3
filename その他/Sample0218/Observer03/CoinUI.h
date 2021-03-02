#pragma once

#include	"Texture.h"

/**
 * コインUIクラス
 */
class CoinUI
{
private:
	/** コイン */
	int coin;
	/** コイン用画像 */
	Texture texture;
public:
	/**
	 * コンストラクタ
	 */
	CoinUI()
		: coin(0)
		, texture()
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
	void Print() const {
		std::cout << texture.GetName() << " : " << coin << std::endl;
	}

	void SetCoin(int val) { coin = val; }
};

