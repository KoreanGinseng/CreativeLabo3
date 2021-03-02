#pragma once

#include	"EnemyFactory.h"
#include	"Goblin.h"

/**
 * �S�u�����̐����H��
 */
class GoblinFactory : public EnemyFactory {
private:
public:
	/**
	 * @brief	�G�̐����������Ȃ����߂̃��\�b�h
	 */
	virtual Enemy* Create() const { return new Goblin(); }
};