#include	"Cpu.h"

/**
 * コンストラクタ
 */
CCpu::CCpu() :
CCharacter() ,
m_CpuTime(0) {
}

/**
 * デストラクタ
 */
CCpu::~CCpu(){
	Release();
}

/**
* 初期化
*/
void CCpu::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -20.0f);
	m_Angle = Vector3(0.0f, MOF_MATH_PI, 0.0f);
	m_pMotion->ChangeMotionByName("bse_wait_lp", 1.0f, TRUE);

	//カメラの設定
	m_Camera.SetViewPort();
	m_Camera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 2000.0f);
	UpdateCamera();

	//実行する関数の登録
	ConditionNode* temp = new ConditionNode();
	temp->m_ConditionTask = &CCpu::CpuDistance;

	ConditionNode* temp2 = new ConditionNode();
	temp2->m_ConditionTask = &CCpu::CpuIsEnemyAttack;

	ActionNode* act1 = new ActionNode();
	act1->m_Task = &CCpu::CpuGuard;

	ActionNode* act2 = new ActionNode();
	act2->m_Task = &CCpu::CpuAttack;

	ActionNode* act3 = new ActionNode();
	act3->m_Task = &CCpu::CpuMove;

	temp->Yes = temp2;
	temp->No = act3;

	temp2->Yes = act1;
	temp2->No = act2;

	root = temp;
}

/**
 * 更新
 *
 * 引数
 * [in]			chara			ターゲットキャラ
 * [in]			pMesh			判定メッシュ
 */
void CCpu::Update(CCharacter& chara,LPMeshContainer pMesh){
	//ターゲット登録
	m_pTarget = &chara;
	//対象キャラへのベクトルを計算する
	CVector3 tv = m_pTarget->GetPosition() - m_Pos;
	float tl = tv.Length();
	//カメラは常に相手の方向に向ける
	m_CameraAngle = atan2(tv.z, -tv.x) + MOF_MATH_HALFPI;
	//一定時間ごとに動作更新
	m_CpuTime -= GAMEFRAMETIME;
	if (m_CpuTime <= 0)
	{
		root->Exec(this);
		//思考時間を再設定
		m_CpuTime = 0.1f + tl / 20.0f;
	}
	if (!m_bAttack && !m_bGuard)
	{
		m_bMove = true;
	}
	CCharacter::Update(pMesh);
}

bool CCpu::CpuDistance() {
	//対象キャラへのベクトルを計算する
	CVector3 tv = m_pTarget->GetPosition() - m_Pos;
	float tl = tv.Length();
	return tl <= 2.5f;
}
bool CCpu::CpuIsEnemyAttack() {
	return m_pTarget->IsAttack() && !m_bJump;
}

void CCpu::CpuGuard() {
	m_MoveAngle = MOF_ToRadian(0.0f);
	m_bGuard = true;
}
void CCpu::CpuAttack() {
	m_MoveAngle = MOF_ToRadian(0.0f);
	m_bGuard = false;
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
void CCpu::CpuMove() {
	//乱数で斜めの入力を決定する
	int rm = CUtilities::Random(3);
	if (rm == 0)
	{
		//相手の方向に前進する
		m_MoveAngle = MOF_ToRadian(0.0f);
	}
	else if (rm == 1)
	{
		//右前方方向に移動する
		m_MoveAngle = MOF_ToRadian(45.0f);
	}
	else if (rm == 2)
	{
		//左前方方向に移動する
		m_MoveAngle = MOF_ToRadian(315.0f);
	}
	m_bGuard = false;
}