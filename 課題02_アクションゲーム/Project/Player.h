#pragma once

#include	"Character.h"

// 回転速度
#define		PLAYER_CAMERAROTATIONSPEED	(0.02f)

class CPlayer : public CCharacter {
private:
	//---------------------------------------------------
	//課題１追加	ロックオン動作の追加
	//
	//m_bLockOn		ロックオンフラグ
	//				TRUEの場合敵の方向にカメラを向ける
	//---------------------------------------------------
	bool				m_bLockOn;
public:
	CPlayer();
	virtual ~CPlayer();
	virtual void Update(CCharacter& chara, LPMeshContainer pMesh);
};