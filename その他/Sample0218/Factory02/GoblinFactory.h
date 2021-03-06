#pragma once

#include	"EnemyFactory.h"
#include	"Goblin.h"

/**
 * ゴブリンの生成工場
 */
class GoblinFactory : public EnemyFactory {
private:
public:
	/**
	 * @brief	敵の生成をおこなうためのメソッド
	 */
	virtual Enemy* Create() const { return new Goblin(); }
};