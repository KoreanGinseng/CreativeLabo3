#pragma once

#include	<iostream>

/**
 * �G�ɋ��ʂ�����N���X
 */
class Enemy {
protected:
	/** �L�����N�^�[�̈ʒu */
	int posX;
	int posY;
public:
	/**
	 * �R���X�g���N�^
	 */
	Enemy()
		: posX(0)
		, posY(0)
	{
	}
	/**
	 * �f�X�g���N�^
	 */
	virtual ~Enemy()
	{
	}

	/**
	 * @brief	���W�̐ݒ�
	 */
	void SetPosition(int px, int py)
	{
		posX = px;
		posY = py;
	}
};