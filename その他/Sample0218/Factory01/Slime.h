#pragma once

#include	"Enemy.h"

class Slime : public Enemy {
private:
public:
	/**
	 * �R���X�g���N�^
	 */
	Slime()
		: Enemy()
	{
		std::cout << "�X���C���������ꂽ!!" << std::endl;
	}
	/**
	 * �f�X�g���N�^
	 */
	virtual ~Slime()
	{
		std::cout << "�X���C�����j�����ꂽ!!" << std::endl;
	}
};