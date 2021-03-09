#include	"Player.h"

/**
 * �R���X�g���N�^
 */
CPlayer::CPlayer() :
CCharacter() ,
m_bLockOn(false) {
}

/**
 * �f�X�g���N�^
 */
CPlayer::~CPlayer(){
	Release();
}

/**
 * �X�V
 *
 * ����
 * [in]			chara			�^�[�Q�b�g�L����
 * [in]			pMesh			���胁�b�V��
 */
void CPlayer::Update(CCharacter& chara, LPMeshContainer pMesh){
	//�L�[���͂ňړ�
	if (g_pInput->IsKeyHold(MOFKEY_LEFT) && g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_MoveAngle = MOF_ToRadian(315.0f);
		m_bMove = true;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_LEFT) && g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_MoveAngle = MOF_ToRadian(225.0f);
		m_bMove = true;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT) && g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_MoveAngle = MOF_ToRadian(45.0f);
		m_bMove = true;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT) && g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_MoveAngle = MOF_ToRadian(135.0f);
		m_bMove = true;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		m_MoveAngle = MOF_ToRadian(270.0f);
		m_bMove = true;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		m_MoveAngle = MOF_ToRadian(90.0f);
		m_bMove = true;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		m_MoveAngle = MOF_ToRadian(0.0f);
		m_bMove = true;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		m_MoveAngle = MOF_ToRadian(180.0f);
		m_bMove = true;
	}

	//---------------------------------------------------
	//�ۑ�P���b�N�I���t���O�̐؂�ւ�
	//		�u�v�v�L�[�̓��͂Ń��b�N�I���t���O��؂�ւ�
	//---------------------------------------------------

	//---------------------------------------------------
	//�ۑ�P�G�ւ̃��b�N�I���t���O���n�m�̏ꍇ�͓G�̕����ɏ�ɃJ������������
	//		�J������G�ւ̕����Ɍ����鏈����CCpu��Update�֐����Q�l�ɂ���B
	//---------------------------------------------------
	
	if (g_pInput->IsKeyHold(MOFKEY_Q))
	{
		m_CameraAngle -= PLAYER_CAMERAROTATIONSPEED;
	}
	else if (g_pInput->IsKeyHold(MOFKEY_E))
	{
		m_CameraAngle += PLAYER_CAMERAROTATIONSPEED;
	}

	if (g_pInput->IsKeyPush(MOFKEY_SPACE))
	{
		if (!m_bAttack)
		{
			StartAttack();
		}
		else
		{
			//���̓o�^
			m_bNextAttack = true;
		}
	}

	if (g_pInput->IsKeyHold(MOFKEY_Z) && !m_bJump)
	{
		m_bGuard = true;
	}
	else
	{
		m_bGuard = false;
	}

	if (g_pInput->IsKeyHold(MOFKEY_X) && !m_bJump)
	{
		StartJump();
	}
	CCharacter::Update(pMesh);
}