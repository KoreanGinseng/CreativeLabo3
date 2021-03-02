#pragma once

#include	"Texture.h"
#include	"Status.h"

/**
 * �L�����N�^�[�N���X
 */
class Character
{
private:
	/** �L�����N�^�[�͉摜���Q�Ƃ��� */
	Texture* texture;

	/** �X�e�[�^�X�����ʂŎQ�Ƃ��� */
	Status* status;

	/** �L�����N�^�[�̈ʒu */
	int posX;
	int posY;
public:
	/**
	 * �R���X�g���N�^
	 */
	Character()
		: texture(nullptr)
		, status(nullptr)
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
	 * @brief		�X�e�[�^�X�̐ݒ�
	 */
	void SetStatus(Status* st)
	{
		std::cout << "�L�����N�^�ɗ��p����X�e�[�^�X��ݒ�" << std::endl;
		status = st;
	}

	/**
	 * @brief		�G�̕\��
	 */
	void Render()
	{
		std::cout << "�摜[" << texture->GetName() << "]��\��" << std::endl;
	}
};

