#pragma once

#include	"Character.h"

// ��]���x
#define		PLAYER_CAMERAROTATIONSPEED	(0.02f)

class CPlayer : public CCharacter {
private:
	//---------------------------------------------------
	//�ۑ�P�ǉ�	���b�N�I������̒ǉ�
	//
	//m_bLockOn		���b�N�I���t���O
	//				TRUE�̏ꍇ�G�̕����ɃJ������������
	//---------------------------------------------------
	bool				m_bLockOn;
public:
	CPlayer();
	virtual ~CPlayer();
	virtual void Update(CCharacter& chara, LPMeshContainer pMesh);
};