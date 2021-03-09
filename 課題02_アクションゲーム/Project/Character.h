#pragma once

#include	"GameDefine.h"

// 移動速度
#define		CHARACTER_MOVESPEED			(0.015f)
#define		CHARACTER_MOVESPEEDMAX		(0.25f)
// 回転速度
#define		CHARACTER_ROTATIONSPEED		(0.3f)
//攻撃回数
#define		CHARACTER_ATTACKCOUNT		(3)
#define		CAHRACTER_ATTACKMOVETIME	(0.1f)

class CCharacter {
protected:
	CMeshContainer				m_Mesh;
	LPMeshMotionController		m_pMotion;
	CVector3					m_Pos;
	CVector3					m_Move;
	CVector3					m_Angle;

	CCamera						m_Camera;
	float						m_CameraAngle;

	bool						m_bMove;
	float						m_MoveAngle;

	float						m_Gravity;
	bool						m_bGround;
	bool						m_bStage;

	CMeshContainer				m_WeaponMesh;

	bool						m_bAttack;
	bool						m_bNextAttack;
	bool						m_bAttackEnd;
	int							m_AttackCount;
	float						m_AttackTime;

	bool						m_bDamage;
	bool						m_bDown;
	float						m_Invincible;

	bool						m_bGuard;

	//---------------------------------------------------
	//課題２追加	ジャンプ動作の追加
	//
	//m_bJump		ジャンプフラグ
	//				ジャンプ中ならTRUE
	//
	//m_bJumpStart	ジャンプ開始フラグ
	//				開始踏み込みアニメーション中ならTRUE
	//
	//m_bJumpEnd	ジャンプ終了フラグ
	//				着地アニメーション中ならTRUE
	//---------------------------------------------------
	bool						m_bJump;
	bool						m_bJumpStart;
	bool						m_bJumpEnd;
public:
	CCharacter();
	virtual ~CCharacter();
	virtual bool Load();
	virtual void Initialize();
	virtual void Update(LPMeshContainer pMesh);
	virtual void UpdateMove();
	virtual void StartAttack();
	virtual void UpdateAttack();
	virtual void StartDamage(CCharacter& chara);
	virtual void UpdateDamage();
	virtual void StartJump();
	virtual void UpdateJump();
	virtual void UpdateCamera();
	virtual void CollisionStage(LPMeshContainer pMesh);
	virtual void CollisionCharacter(CCharacter& chara);
	virtual bool CollisionGuard(CCharacter& chara);
	virtual void SetCameraEnable();
	virtual void Render();
	virtual void RenderDebug();
	virtual void RenderDebugText();
	virtual void Release();
	CSphere GetSphere(){ return CSphere(m_Pos.x, m_Pos.y + 0.5f, m_Pos.z, 1.0f); }
	CSphere GetAttackSphere();
	CVector3& GetPosition(){ return m_Pos; }
	bool IsAttack(){ return ((m_bAttack && !m_bAttackEnd) ? true : false); }
};