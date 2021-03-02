#pragma once

#include	"EnemyFactory.h"
#include	"Slime.h"

/**
 * スライムの生成工場
 */
class SlimeFactory : public EnemyFactory {
private:
public:
	/**
	 * @brief	敵の生成をおこなうためのメソッド
	 */
	virtual Enemy* Create() const { return new Slime(); }
};