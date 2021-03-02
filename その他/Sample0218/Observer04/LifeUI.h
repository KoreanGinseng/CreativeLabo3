#pragma once

#include	"Texture.h"

/**
 * �c�@UI�N���X
 */
class LifeUI : public Observer<int>
{
private:
	/** �c�@ */
	int life;
	/** �c�@�p�摜 */
	Texture texture;
public:
	/**
	 * �R���X�g���N�^
	 */
	LifeUI()
		: life(3)
		, texture()
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	~LifeUI()
	{
	}

	/**
	 * @brief		�摜�̓ǂݍ��ݏ���
	 */
	bool Load()
	{
		return texture.Load("Life.png");
	}

	/**
	 * @brief	�c�@��\������
	 */
	void Print() const {
		std::cout << texture.GetName() << " : " << life << std::endl;
	}

	void SetLife(int val) { life = val; }

	/**
	 * @brief	�ʒm���\�b�h
	 */
	void Notify(int val) override {
		SetLife(val);
	}
};

