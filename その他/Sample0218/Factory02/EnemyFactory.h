#pragma once

#include	"Enemy.h"

/**
 * “G‚Ì¶¬Hê
 */
class EnemyFactory {
private:
public:
	/**
	 * @brief	“G‚Ì¶¬‚ğ‚¨‚±‚È‚¤‚½‚ß‚Ìƒƒ\ƒbƒh
	 */
	virtual Enemy* Create() const { return nullptr; }
};