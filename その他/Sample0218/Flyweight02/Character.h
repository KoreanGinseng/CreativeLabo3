#pragma once

#include	"Texture.h"

/**
 * �L�����N�^�[�N���X
 */
class Character
{
private:
	/** �L�����N�^�[�͉摜���Q�Ƃ��� */
	Texture* texture;

	/** �L�����N�^�[�̃X�e�[�^�X */
	int maxHP;
	int maxMP;
	int STR;
	int DEF;
	int SPD;

	/** �L�����N�^�[�̈ʒu */
	int posX;
	int posY;
public:
	/**
	 * �R���X�g���N�^
	 */
	Character()
		: texture(nullptr)
		, maxHP(10)
		, maxMP(10)
		, STR(5)
		, DEF(5)
		, SPD(5)
		, posX(0)
		, posY(0)
	{
		std::cout << "�L�����N�^�����ꂽ!!" << std::endl;
	}
	/**
	 * �f�X�g���N�^
	 */
	~Character()
	{
		std::cout << "�L�����N�^���j�����ꂽ!!" << std::endl;
	}

	/**
	 * @brief		�摜�̐ݒ�
	 */
	void SetTexture(Texture* tex)
	{
		std::cout << "�L�����N�^�ɗ��p����摜��ݒ�" << std::endl;
		texture = tex;
	}

	/**
	 * @brief		�G�̕\��
	 */
	void Render()
	{
		std::cout << "�摜[" << texture->GetName() << "]��\��" << std::endl;
	}
};

