#include	"Character.h"

/**
 * コンストラクタ
 */
CCharacter::CCharacter() :
m_Mesh() ,
m_pMotion(NULL) ,
m_Pos() ,
m_Move() ,
m_Angle() ,
m_Camera() ,
m_CameraAngle(0) ,
m_bMove(false) ,
m_MoveAngle(0) ,
m_Gravity(0) ,
m_bGround(false) ,
m_bStage(false) ,
m_WeaponMesh() ,
m_bAttack(false) ,
m_bNextAttack(false) ,
m_bAttackEnd(false) ,
m_AttackCount(0) ,
m_AttackTime(0) ,
m_bDamage(false) ,
m_bDown(false) ,
m_Invincible(0) ,
m_bGuard(false) ,
m_bJump(false) ,
m_bJumpStart(false) ,
m_bJumpEnd(false) {
}

/**
 * デストラクタ
 */
CCharacter::~CCharacter(){
}

/**
 * 読み込み
 */
bool CCharacter::Load(void){
	// メッシュの読み込み
	if(!m_Mesh.Load("Chr_01_ion_mdl_01.mom"))
	{
		return false;
	}
	m_pMotion = m_Mesh.CreateMotionController();

	//武器メッシュの読み込み
	if (!m_WeaponMesh.Load("Wep_01_ion_mdl_01.mom"))
	{
		return false;
	}
	return true;
}

/**
 * 初期化
 */
void CCharacter::Initialize(void){
	m_Pos = Vector3(0.0f,0.0f,0.0f);
	m_pMotion->ChangeMotionByName("bse_wait_lp", 1.0f, TRUE);

	//カメラの設定
	m_Camera.SetViewPort();
	m_Camera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 2000.0f);
	UpdateCamera();
}

/**
 * 更新
 *
 * 引数
 * [in]			pMesh			判定メッシュ
 */
void CCharacter::Update(LPMeshContainer pMesh){
	//無敵時間があれば減らす
	if (m_Invincible > 0.0f)
	{
		m_Invincible -= GAMEFRAMETIME;
	}
	if (m_bDamage)
	{
		//ダメージ更新
		UpdateDamage();
	}
	else if (m_bAttack)
	{
		//攻撃更新
		UpdateAttack();
	}
	else if (m_bJump)
	{
		//ジャンプ更新
		UpdateJump();
	}
	else if (m_bGuard)
	{
		m_pMotion->ChangeMotionByName("act_guard", 1.0f, TRUE, FALSE);
		m_bMove = false;
	}
	//移動更新
	UpdateMove();

	//ステージと接触判定
	CollisionStage(pMesh);

	//---------------------------------------------------
	//課題４ジャンプ攻撃動作の追加
	//		ジャンプ攻撃中は重力の落下速度を半分にする
	//---------------------------------------------------
	m_Gravity -= GRAVITY;

	//座標更新
	m_Pos.y += m_Gravity;
	if (!m_bStage)
	{
		m_Pos += m_Move;
	}

	//アニメーションの時間を進める
	m_pMotion->AddTimer(GAMEFRAMETIME);

	//カメラ更新
	UpdateCamera();
}

/**
 * 移動更新
 */
void CCharacter::UpdateMove(void){
	if (m_bMove)
	{
		if (!m_bAttack && !m_bDamage && !m_bGuard && !m_bJump)
		{
			m_pMotion->ChangeMotionByName("mov_run_lp", 1.0f, TRUE, FALSE);
		}

		//カメラの前方向のベクトル
		CVector3 cfvec = m_Camera.GetViewFront();
		//カメラのY軸の回転角度を求める
		float cy = atan2(cfvec.z, -cfvec.x) + MOF_MATH_HALFPI;
		//移動角度を求める
		float my = m_MoveAngle + cy;
		MOF_NORMALIZE_RADIANANGLE(my);
		//差分角度
		float sa = my - m_Angle.y;
		MOF_ROTDIRECTION_RADIANANGLE(sa);
		//回転
		m_Angle.y += MOF_CLIPING(sa, -CHARACTER_ROTATIONSPEED, CHARACTER_ROTATIONSPEED);
		MOF_NORMALIZE_RADIANANGLE(m_Angle.y);

		//移動方向のベクトル
		CVector3 fvec(0, 0, -1);
		fvec.RotationY(my);
		m_Move += fvec * CHARACTER_MOVESPEED;
		//移動を最高速度でクリップする
		float ml = m_Move.Length();
		if (ml >= CHARACTER_MOVESPEEDMAX)
		{
			m_Move.Normal(m_Move);
			m_Move *= CHARACTER_MOVESPEEDMAX;
		}
	}
	else
	{
		float ml = m_Move.Length();
		if (ml > CHARACTER_MOVESPEED)
		{
			m_Move.Normal(m_Move);
			m_Move *= (ml - CHARACTER_MOVESPEED);
		}
		else
		{
			m_Move.SetValue(0.0f, 0.0f, 0.0f);
		}
		if (!m_bAttack && !m_bDamage && !m_bGuard && !m_bJump)
		{
			m_pMotion->ChangeMotionByName("bse_wait_lp", 1.0f, TRUE, FALSE);
		}
	}
	m_bMove = false;
}

/**
 * 攻撃開始
 */
void CCharacter::StartAttack(){
	//ダメージ中は動作開始できない
	if (m_bDamage)
	{
		return;
	}
	//ジャンプの踏み込みや着地中は動作開始できない
	if (m_bJumpStart || m_bJumpEnd)
	{
		return;
	}
	//攻撃状態の設定
	if (!m_bAttack)
	{
		m_bAttack = true;
		m_bNextAttack = false;
		m_AttackCount = 0;
	}
	else
	{
		m_bNextAttack = false;
		m_AttackCount++;
	}
	m_AttackTime = 0;
	m_bAttackEnd = false;

	//---------------------------------------------------
	//課題４ジャンプ攻撃動作の追加
	//		m_bJumpフラグを確認しジャンプ中なら、重力値を0.1に設定
	//		再生するアニメーションを下記に変更
	//		攻撃１段目：atk_jump_01
	//		攻撃２段目：atk_jump_02
	//		攻撃３段目：atk_jump_03
	//---------------------------------------------------

	//通常攻撃のアニメーション名定義
	static char* aname[] = {
		"atk_attack_01",
		"atk_attack_02",
		"atk_attack_03",
	};
	//通常攻撃のアニメーションへ変更
	m_pMotion->ChangeMotionByName(aname[m_AttackCount], 1.0f, FALSE, FALSE);
}

/**
 * 攻撃更新
 */
void CCharacter::UpdateAttack(){
	m_AttackTime += GAMEFRAMETIME;
	if (m_AttackTime < CAHRACTER_ATTACKMOVETIME)
	{
		m_bMove = true;
	}
	else
	{
		m_bMove = false;
	}
	m_bGuard = false;
	//攻撃はアニメーション終了で次の動作へ
	if (m_pMotion->IsEndMotion())
	{
		//次の動作が登録済み
		if (m_bNextAttack && m_AttackCount + 1 < CHARACTER_ATTACKCOUNT)
		{
			StartAttack();
		}
		//攻撃の終了
		else if (m_bAttackEnd)
		{
			m_bAttack = false;
		}
		else
		{
			//終了アニメーション
			m_bAttackEnd = true;

			//---------------------------------------------------
			//課題４ジャンプ攻撃終了動作の追加
			//		m_bJumpフラグを確認しジャンプ中なら
			//		再生するアニメーションを下記に変更
			//		攻撃１段目：atk_jump_01_ft
			//		攻撃２段目：atk_jump_02_ft
			//		攻撃３段目：atk_jump_03_ft
			//---------------------------------------------------

			//通常攻撃のアニメーション名定義
			static char* aname[] = {
				"atk_attack_01_ft",
				"atk_attack_02_ft",
				"atk_attack_03_ft",
			};
			//通常攻撃のアニメーションへ変更
			m_pMotion->ChangeMotionByName(aname[m_AttackCount], 1.0f, FALSE, FALSE);
		}
	}
}

/**
 * ダメージ開始
 *
 * 引数
 * [in]			chara			判定キャラ
 */
void CCharacter::StartDamage(CCharacter& chara){
	//ほかの動作は中断
	m_bAttack = false;
	m_bNextAttack = false;
	m_bJump = false;
	//攻撃の最終段でダウンさせる
	if (chara.m_AttackCount >= CHARACTER_ATTACKCOUNT - 1)
	{
		//ダメージの開始と無敵時間の設定
		m_bDamage = true;
		m_bDown = true;
		m_Invincible = 1.5f;
		//相手キャラからの攻撃方向にノックバック
		m_Move = m_Pos - chara.m_Pos;
		m_Move.Normal(m_Move);
		m_Move *= 0.3f;
		m_Move.y += 0.3f;
		//アニメーション変更
		m_pMotion->ChangeMotionByName("dam_down_u", 1.0f, FALSE, TRUE);
	}
	else
	{
		//ダメージの開始と無敵時間の設定
		m_bDamage = true;
		m_bDown = false;
		m_Invincible = 0.4f;
		static char* aname[] = {
			"dam_damage_m_a",
			"dam_damage_m_b",
			"dam_damage_m_c",
		};
		//相手キャラからの攻撃方向にノックバック
		m_Move = m_Pos - chara.m_Pos;
		m_Move.Normal(m_Move);
		m_Move *= 0.15f;
		//アニメーション変更
		m_pMotion->ChangeMotionByName(aname[CUtilities::Random(3)], 1.0f, FALSE, TRUE);
	}
}

/**
 * ダメージ更新
 */
void CCharacter::UpdateDamage(){
	//その他の動作をキャンセル
	m_bAttack = false;
	m_bGuard = false;
	m_bMove = false;
	m_bJump = false;
	m_bJumpEnd = false;
	m_bJumpStart = false;
	//ノックバックは徐々に減速
	m_Move *= 0.98f;
	if (m_bDown)
	{
		//ダウンはアニメーション終了でダメージへ
		if (m_pMotion->IsEndMotion() && m_Invincible <= 0.0f)
		{
			m_bDown = false;
			m_Invincible = 1.0f;
			m_pMotion->ChangeMotionByName("dam_down_u_standup", 1.0f, FALSE, TRUE);
		}
	}
	else
	{
		//ダメージはアニメーション終了で次の動作へ
		if (m_pMotion->IsEndMotion())
		{
			m_bDamage = false;
		}
	}
}

/**
 * ジャンプ開始
 */
void CCharacter::StartJump(){
	//ダメージ中や攻撃中はジャンプできない
	if (m_bDamage || m_bAttack)
	{
		return;
	}

	//---------------------------------------------------
	//課題２ジャンプ開始処理の追加
	//		ジャンプフラグとジャンプ開始フラグをtrue
	//		ジャンプ終了フラグをfalseにして"mov_jump_f_set"にアニメーションを変更
	//---------------------------------------------------
}

/**
 * ジャンプ更新
 */
void CCharacter::UpdateJump(){
	//---------------------------------------------------
	//課題２ジャンプ処理の追加
	//		ジャンプ開始中："mov_jump_f_set"のアニメーション終了で、上昇開始
	//						重力を0.3に設定、ジャンプ開始フラグをfalseにして、アニメーションを"mov_jump_f_u_lp"へ変更
	//
	//		ジャンプ終了中："mov_jump_landing"のアニメーション終了で
	//						ジャンプフラグとジャンプ終了フラグをfalseにして、ジャンプ終了
	//
	//		ジャンプ中：	落下状態（重力がー）になった場合、
	//						アニメーションを"mov_jump_f_d_lp"へ変更
	//---------------------------------------------------
}

/**
 * カメラ更新
 */
void CCharacter::UpdateCamera(){
	//カメラの設定
	CVector3 cpos = m_Pos;
	CVector3 tpos = m_Pos;
	CVector3 fvec(0, 0, -1);
	fvec.RotationY(m_CameraAngle);
	cpos.y += 2.0f;
	cpos -= fvec * 3.2f;
	tpos += fvec * 0.5f;
	m_Camera.LookAt(cpos, tpos, CVector3(0, 1, 0));
	m_Camera.Update();
}

/**
 * ステージのメッシュと当たり判定をおこなう
 *
 * 引数
 * [in]			pMesh			判定メッシュ
 */
void CCharacter::CollisionStage(LPMeshContainer pMesh){
	//判定初期化
	m_bGround = false;
	m_bStage = false;
	//接触情報
	COLLISIONOUTGEOMETRY gout;
	//進行方向へのレイ
	CRay3D mray(m_Pos, m_Move);
	mray.Position.y += 0.5f;
	if (mray.CollisionMesh(pMesh, gout))
	{
		if (gout.d <= m_Move.Length() + 1.0f)
		{
			float nd = CVector3Utilities::Dot(m_Move, gout.Normal);
			nd = MOF_ABS(nd);
			m_Pos += mray.Direction * (gout.d - (1.0f - nd));
			m_Pos += gout.Normal * nd;
			m_bStage = true;
		}
	}
	//下方向へのレイ
	CRay3D dray(m_Pos, CVector3(0, -1, 0));
	dray.Position.y += 2.0f;
	if(dray.CollisionMesh(pMesh, gout))
	{
		if (gout.d <= 2.1f)
		{
			m_Pos.y += 2.1f - gout.d;

			//---------------------------------------------------
			//課題２地面への接触で着地モーションを実行
			//		ジャンプ落下中に地面に接触した場合、
			//		ジャンプ終了フラグをtrueにしてアニメーションを"mov_jump_landing"へ変更
			//---------------------------------------------------
			
			//通常状態での地面への接触処理
			if (m_Gravity < 0)
			{
				m_Gravity = 0.0f;
				m_bGround = true;
			}
		}
	}
}

/**
 * キャラクターと当たり判定をおこなう
 *
 * 引数
 * [in]			chara			判定キャラ
 */
void CCharacter::CollisionCharacter(CCharacter& chara){
	//球体を取得して当たり判定
	CSphere p1 = GetSphere();
	CSphere p2 = chara.GetSphere();
	if (p1.CollisionSphere(p2))
	{
		//埋まり防止をおこなう
		CVector3 sv = p1.Position - p2.Position;
		float sl = (p1.r + p2.r) - sv.Length();
		sv.Normal(sv);
		m_Pos += sv * sl * 0.5f;
		chara.m_Pos -= sv * sl * 0.5f;
	}

	//お互いの攻撃の当たり判定
	if (m_bAttack && !m_bAttackEnd && chara.m_Invincible <= 0.0f)
	{
		CSphere ap1 = GetAttackSphere();
		if (ap1.CollisionSphere(p2) && !chara.CollisionGuard(*this))
		{
			chara.StartDamage(*this);
		}
	}
	if (chara.m_bAttack && !chara.m_bAttackEnd && m_Invincible <= 0.0f)
	{
		CSphere ap2 = chara.GetAttackSphere();
		if (p1.CollisionSphere(ap2) && !CollisionGuard(chara))
		{
			StartDamage(chara);
		}
	}
}

/**
 * キャラクターのガード判定をおこなう
 *
 * 引数
 * [in]			chara			判定キャラ
 */
bool CCharacter::CollisionGuard(CCharacter& chara){
	//ガード中は接触があっても前方からの攻撃なら無効にする
	if (!m_bGuard)
	{
		return false;
	}
	//相手へのベクトルと自分の前方ベクトルの内積が正（>０）の場合は鋭角であり前方９０°以内と判断できる
	CVector3 ev = chara.m_Pos - m_Pos;
	CVector3 fv(0, 0, -1);
	CMatrix33 matRot;
	matRot.RotationZXY(m_Angle);
	fv *= matRot;
	if (fv.Dot(ev) > 0.0f)
	{
		//ガード成功
		return true;
	}
	return false;
}

/**
 * カメラを有効にする
 */
void CCharacter::SetCameraEnable(){
	CGraphicsUtilities::SetCamera(&m_Camera);
}

/**
 * 描画
 */
void CCharacter::Render(void){
	// ワールド行列作成
	CMatrix44 matWorld;
	matWorld.RotationZXY(m_Angle);
	matWorld.SetTranslation(m_Pos);
	//アニメーションにワールドマトリクスを適用
	m_pMotion->RefreshBoneMatrix(matWorld);
	// メッシュの描画
	m_Mesh.Render(m_pMotion);

	//武器を設定するボーンの情報を取得する
	LPBONEMOTIONSTATE pBoneState = m_pMotion->GetBoneState("UPP_weapon");
	if (!pBoneState)
	{
		return;
	}
	//武器メッシュを描画する行列をボーン情報から計算する
	CMatrix44 matWeapon = pBoneState->pBone->GetRotationOffsetMatrix() * pBoneState->BoneMatrix;
	//武器メッシュの描画
	m_WeaponMesh.Render(matWeapon);
}

/**
 * デバッグ描画
 */
void CCharacter::RenderDebug(void){
	// 当たり判定の表示
	CGraphicsUtilities::RenderLineSphere(GetSphere(),Vector4(0,1,0,0.3f));
	// 攻撃当たり判定の表示
	CGraphicsUtilities::RenderLineSphere(GetAttackSphere(), Vector4(1, 0, 0, 0.3f));
}

/**
 * デバッグ文字描画
 */
void CCharacter::RenderDebugText(void){
	// 位置の描画
	CGraphicsUtilities::RenderString(10,40,MOF_COLOR_WHITE,
			"プレイヤー位置 X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
 * 解放
 */
void CCharacter::Release(void){
	MOF_SAFE_DELETE(m_pMotion);
	m_Mesh.Release();
	m_WeaponMesh.Release();
}

/**
 * 攻撃の当たり判定用球体取得
 */
CSphere CCharacter::GetAttackSphere(){
	//武器を設定するボーンの情報を取得する
	LPBONEMOTIONSTATE pBoneState = m_pMotion->GetBoneState("UPP_weapon");
	if (!pBoneState)
	{
		return CSphere(m_Pos, 0.5f);
	}
	//武器メッシュを描画する行列をボーン情報から計算する
	CMatrix44 matWeapon = pBoneState->pBone->GetRotationOffsetMatrix() * pBoneState->BoneMatrix;
	CVector3 pos;
	matWeapon.GetTranslation(pos);
	CVector3 offset(0, 0, -1);
	CMatrix33 rmat;
	matWeapon.GetRotation(rmat);
	offset *= rmat;
	pos += offset;
	return CSphere(pos, 0.5f);
}