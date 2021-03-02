#pragma once

#include	"Character.h"

/**
 * スコアUIクラス
 */
class ScoreUI
{
private:
	/** スコア用画像 */
	Texture texture;
public:
	/**
	 * コンストラクタ
	 */
	ScoreUI()
		: texture()
	{
	}
	/**
	 * デストラクタ
	 */
	~ScoreUI()
	{
	}

	/**
	 * @brief		画像の読み込み処理
	 */
	bool Load()
	{
		return texture.Load("Score.png");
	}

	/**
	 * @brief	スコアを表示する
	 */
	void Print(const Character& chara) const {
		std::cout << texture.GetName() << " : " << chara.GetScore() << std::endl;
	}
};

