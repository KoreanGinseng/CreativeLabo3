#pragma once

#include	"Enemy.h"

/**
 * �G�̐����H��
 */
class EnemyFactory {
private:
public:
	/**
	 * @brief	�G�̐����������Ȃ����߂̃��\�b�h
	 */
	virtual Enemy* Create() const { return nullptr; }
};