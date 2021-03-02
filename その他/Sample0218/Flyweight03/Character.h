#pragma once

#include	"Texture.h"
#include	"Status.h"

/**
 * キャラクタークラス
 */
class Character
{
private:
	/** キャラクターは画像を参照する */
	Texture* texture;

	/** ステータスも共通で参照する */
	Status* status;

	/** キャラクターの位置 */
	int posX;
	int posY;
public:
	/**
	 * コンストラクタ
	 */
	Character()
		: texture(nullptr)
		, status(nullptr)
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
	 * @brief		ステータスの設定
	 */
	void SetStatus(Status* st)
	{
		std::cout << "キャラクタに利用するステータスを設定" << std::endl;
		status = st;
	}

	/**
	 * @brief		敵の表示
	 */
	void Render()
	{
		std::cout << "画像[" << texture->GetName() << "]を表示" << std::endl;
	}
};

