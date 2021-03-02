#pragma once

#include	"Texture.h"
#include	"Status.h"

/**
 * 敵に共通する基底クラス
 */
class Enemy {
protected:
	/** キャラクターは画像を参照する */
	TexturePtr texture;

	/** ステータスも共通で参照する */
	StatusPtr status;

	/** キャラクターの位置 */
	int posX;
	int posY;
public:
	/**
	 * コンストラクタ
	 */
	Enemy()
		: posX(0)
		, posY(0)
	{
	}
	/**
	 * デストラクタ
	 */
	virtual ~Enemy()
	{
	}

	/**
	 * @brief		敵の表示
	 */
	void Render()
	{
		std::cout << "画像[" << texture->GetName() << "]を表示" << std::endl;
	}

	/**
	 * @brief	座標の設定
	 */
	void SetPosition(int px, int py)
	{
		posX = px;
		posY = py;
	}
};