#pragma once

#include	<iostream>

/**
 * 敵に共通する基底クラス
 */
class Enemy {
protected:
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
	 * @brief	座標の設定
	 */
	void SetPosition(int px, int py)
	{
		posX = px;
		posY = py;
	}
};