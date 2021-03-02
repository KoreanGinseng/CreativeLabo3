#pragma once

#include	<iostream>

/**
 * 画像を保持するクラス
 */
class Texture
{
private:
	/** 画像ファイル名 */
	std::string fileName;
public:
	/**
	 * コンストラクタ
	 */
	Texture()
		: fileName()
	{
		std::cout << "画像が作られた!!" << std::endl;
	}
	/**
	 * デストラクタ
	 */
	~Texture()
	{
		std::cout << "画像[" << fileName << "]が破棄された!!" << std::endl;
	}

	/**
	 * @brief		画像の読み込み処理
	 * @param[in]	fname		ファイル名
	 */
	bool Load(const std::string& fname)
	{
		std::cout << "画像[" << fname << "]を読み込んだ!!" << std::endl;
		fileName = fname;
		return true;
	}

	const std::string& GetName() const { return fileName; }
};

