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
		texture = TextureManager::GetInstance().GetTexture("Slime.png");
		status = StatusManager::GetInstance().GetStatus("Slime");
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