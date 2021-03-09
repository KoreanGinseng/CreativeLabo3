#include	"Player.h"

/**
 * コンストラクタ
 */
CPlayer::CPlayer() :
CCharacter() ,
m_bLockOn(false) {
}

/**
 * デストラクタ
 */
CPlayer::~CPlayer(){
	Release();
}

/**
 * 更新
 *
 * 引数
 * [in]			chara			ターゲットキャラ
 * [in]			pMesh			判定メッシュ
 */
void CPlayer::Update(CCharacter& chara, LPMeshContainer pMesh){
	//キー入力で移動
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
	//課題１ロックオンフラグの切り替え
	//		「Ｗ」キーの入力でロックオンフラグを切り替え
	//---------------------------------------------------

	//---------------------------------------------------
	//課題１敵へのロックオンフラグがＯＮの場合は敵の方向に常にカメラを向ける
	//		カメラを敵への方向に向ける処理はCCpuのUpdate関数を参考にする。
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
			//次の登録
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