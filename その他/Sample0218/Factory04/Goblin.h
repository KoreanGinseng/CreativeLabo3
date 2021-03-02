#pragma once

#include	"Enemy.h"

class Goblin : public Enemy {
private:
public:
	/**
	 * コンストラクタ
	 */
	Goblin()
		: Enemy()
	{
		texture = TextureManager::GetInstance().GetTexture("Goblin.png");
		status = StatusManager::GetInstance().GetStatus("Goblin");
		std::cout << "ゴブリンがあらわれた!!" << std::endl;
	}
	/**
	 * デストラクタ
	 */
	virtual ~Goblin()
	{
		std::cout << "ゴブリンが破棄された!!" << std::endl;
	}
};