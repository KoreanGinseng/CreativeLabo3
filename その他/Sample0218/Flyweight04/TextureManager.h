#pragma once

#include	<unordered_map>
#include	"Singleton.h"
#include	"Texture.h"

/**
 * �e�N�X�`���Ǘ�
 */
class TextureManager : public Singleton<TextureManager>
{
private:
	/** �e�N�X�`�� */
	std::unordered_map<std::string, TexturePtr> textures;
public:
	/**
	 * �R���X�g���N�^
	 */
	TextureManager()
		: textures()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~TextureManager()
	{
	}

	/**
	 * @brief		�摜�����݂��Ȃ���Γǂݍ���
	 *				���ɑ��݂���ꍇ�͂��̃|�C���^��Ԃ�
	 * @param[in]	fname		�t�@�C����
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