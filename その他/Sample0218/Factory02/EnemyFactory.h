#pragma once

#include	"Enemy.h"

/**
 * 敵の生成工場
 */
class EnemyFactory {
private:
public:
	/**
	 * @brief	敵の生成をおこなうためのメソッド
	 */
	virtual Enemy* Create() const { return nullptr; }
};