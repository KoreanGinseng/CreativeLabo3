#pragma once

#include	"Texture.h"
#include	"Observer.h"

/**
 * スコアUIクラス
 */
class ScoreUI
{
private:
	/** スコア */
	int score;
	/** スコア用画像 */
	Texture texture;
public:
	/**
	 * コンストラクタ
	 */
	ScoreUI()
		: score(0)
		, texture()
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
	void Print() const {
		std::cout << texture.GetName() << " : " << score << std::endl;
	}

	void SetScore(int val) { score = val; }
};

