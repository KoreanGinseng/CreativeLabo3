#pragma once

#include	"Enemy.h"

class Goblin : public Enemy {
private:
public:
	/**
	 * �R���X�g���N�^
	 */
	Goblin()
		: Enemy()
	{
		std::cout << "�S�u�����������ꂽ!!" << std::endl;
	}
	/**
	 * �f�X�g���N�^
	 */
	virtual ~Goblin()
	{
		std::cout << "�S�u�������j�����ꂽ!!" << std::endl;
	}
};