#pragma once

#include	"Enemy.h"

class Slime : public Enemy {
private:
public:
	/**
	 * コンストラクタ
	 */
	Slime()
		: Enemy()
	{
		std::cout << "スライムがあらわれた!!" << std::endl;
	}
	/**
	 * デストラクタ
	 */
	virtual ~Slime()
	{
		std::cout << "スライムが破棄された!!" << std::endl;
	}
};