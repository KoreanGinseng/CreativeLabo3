#pragma once

#include	"EnemyFactory.h"
#include	"Goblin.h"

/**
 * ƒSƒuƒŠƒ“‚Ì¶¬Hê
 */
class GoblinFactory : public EnemyFactory {
private:
public:
	/**
	 * @brief	“G‚Ì¶¬‚ğ‚¨‚±‚È‚¤‚½‚ß‚Ìƒƒ\ƒbƒh
	 */
	virtual Enemy* Create() const { return new Goblin(); }
};