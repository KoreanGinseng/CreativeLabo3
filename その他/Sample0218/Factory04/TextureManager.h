#pragma once

#include	<unordered_map>
#include	"Singleton.h"
#include	"Texture.h"

/**
 * テクスチャ管理
 */
class TextureManager : public Singleton<TextureManager>
{
private:
	/** テクスチャ */
	std::unordered_map<std::string, TexturePtr> textures;
public:
	/**
	 * コンストラクタ
	 */
	TextureManager()
		: textures()
	{
	}
	/**
	 * デストラクタ
	 */
	~TextureManager()
	{
	}

	/**
	 * @brief		画像が存在しなければ読み込み
	 *				既に存在する場合はそのポインタを返す
	 * @param[in]	fname		ファイル名
	 */
	TexturePtr GetTexture(const std::string& fname) {
		auto obj = textures.find(fname);
		if (obj != textures.end())
		{
			return obj->second;
		}
		TexturePtr re = std::make_shared<Texture>();
		re->Load(fname);
		textures[fname] = re;
		return re;
	}
};