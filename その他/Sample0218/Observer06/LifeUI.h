#pragma once

#include	"Texture.h"

/**
 * 残機UIクラス
 */
class LifeUI : public Observer<int>
{
private:
	/** 残機 */
	int life;
	/** 残機用画像 */
	Texture texture;
public:
	/**
	 * コンストラクタ
	 */
	LifeUI()
		: life(3)
		, texture()
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
	void Print() const {
		std::cout << texture.GetName() << " : " << life << std::endl;
	}

	void SetLife(int val) { life = val; }

	/**
	 * @brief	通知メソッド
	 */
	void Notify(int val) override {
		SetLife(val);
	}
};

