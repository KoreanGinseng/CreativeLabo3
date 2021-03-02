#pragma once

#include	"EnemyFactory.h"
#include	"Slime.h"

/**
 * �X���C���̐����H��
 */
class SlimeFactory : public EnemyFactory {
private:
public:
	/**
	 * @brief	�G�̐����������Ȃ����߂̃��\�b�h
	 */
	virtual Enemy* Create() const { return new Slime(); }
};