#pragma once

#include	"Texture.h"
#include	"Status.h"

/**
 * �G�ɋ��ʂ�����N���X
 */
class Enemy {
protected:
	/** �L�����N�^�[�͉摜���Q�Ƃ��� */
	TexturePtr texture;

	/** �X�e�[�^�X�����ʂŎQ�Ƃ��� */
	StatusPtr status;

	/** �L�����N�^�[�̈ʒu */
	int posX;
	int posY;
public:
	/**
	 * �R���X�g���N�^
	 */
	Enemy()
		: posX(0)
		, posY(0)
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	virtual ~Enemy()
	{
	}

	/**
	 * @brief		�G�̕\��
	 */
	void Render()
	{
		std::cout << "�摜[" << texture->GetName() << "]��\��" << std::endl;
	}

	/**
	 * @brief	���W�̐ݒ�
	 */
	void SetPosition(int px, int py)
	{
		posX = px;
		posY = py;
	}
};