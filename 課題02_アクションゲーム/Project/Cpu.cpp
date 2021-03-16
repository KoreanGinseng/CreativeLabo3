#include	"Cpu.h"
#include    "Fuzzy.h"

/**
 * コンストラクタ
 */
CCpu::CCpu() :
	CCharacter(),
	m_CpuTime(0) {
}

/**
 * デストラクタ
 */
CCpu::~CCpu() {
	Release();
}

/**
* 初期化
*/
void CCpu::Initialize(void) {
	m_Pos = Vector3(0.0f, 0.0f, -20.0f);
	m_Angle = Vector3(0.0f, MOF_MATH_PI, 0.0f);
	m_pMotion->ChangeMotionByName("bse_wait_lp", 1.0f, TRUE);

	//カメラの設定
	m_Camera.SetViewPort();
	m_Camera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 2000.0f);
	UpdateCamera();

	m_Condition[0] = std::bind(&CCpu::AI_Distance     , this);
	m_Condition[1] = std::bind(&CCpu::AI_IsEnemyAttack, this);

	m_Task[0]      = std::bind(&CCpu::AI_Guard        , this);
	m_Task[1]      = std::bind(&CCpu::AI_Attack       , this);
	m_Task[2]      = std::bind(&CCpu::AI_Move         , this);

	std::shared_ptr<ConditionNode> condition1 = std::make_shared<ConditionNode>();
	condition1->m_Condition                   = &CCpu::AI_Distance;
	std::shared_ptr<ConditionNode> condition2 = std::make_shared<ConditionNode>();
	condition2->m_Condition                   = &CCpu::AI_IsEnemyAttack;
	std::shared_ptr<ActionNode> act1          = std::make_shared<ActionNode>();
	act1->m_Task                              = &CCpu::AI_Guard;
	std::shared_ptr<ActionNode> act2          = std::make_shared<ActionNode>();
	act2->m_Task                              = &CCpu::AI_Attack;
	std::shared_ptr<ActionNode> act3          = std::make_shared<ActionNode>();
	act3->m_Task                              = &CCpu::AI_Move;
	condition2->Yes                           = act1;
	condition2->No                            = act2;
	condition1->Yes                           = condition2;
	condition1->No                            = act3;
	root                                      = condition1;
}

/**
 * 更新
 *
 * 引数
 * [in]			chara			ターゲットキャラ
 * [in]			pMesh			判定メッシュ
 */
void CCpu::Update(CCharacter& chara, LPMeshContainer pMesh) {
	m_pTarget = &chara;
	
	//対象キャラへのベクトルを計算する
	CVector3 tv = chara.GetPosition() - m_Pos;
	float tl = tv.Length();
	//カメラは常に相手の方向に向ける
	m_CameraAngle = atan2(tv.z, -tv.x) + MOF_MATH_HALFPI;
	//一定時間ごとに動作更新
	m_CpuTime -= GAMEFRAMETIME;
	if (m_CpuTime <= 0)
	{
		//---------------------------------------------------
		//課題３ＣＰＵのジャンプ処理
		//		ステージメッシュと自分の前方方向のレイによる
		//		当たり判定をおこない3.0以内に壁があればジャンプをさせる
		//---------------------------------------------------
		root->Exec(this);
		//if (m_Condition[0]())
		//{
		//	//相手が攻撃中ならガード、それ以外なら攻撃をおこなう
		//	if (m_Condition[1]())
		//	{
		//		m_Task[0]();
		//	}
		//	else
		//	{
		//		m_Task[1]();
		//	}
		//}
		//else
		//{
		//	m_Task[2]();
		//}

		//思考時間を再設定
		m_CpuTime = 0.1f + tl / 20.0f;
	}
	if (!m_bAttack && !m_bGuard)
	{
		m_bMove = true;
	}
	CCharacter::Update(pMesh);
}

bool CCpu::AI_Distance()
{
	//対象キャラへのベクトルを計算する
	CVector3 tv = m_pTarget->GetPosition() - m_Pos;
	float tl = tv.Length();

	FuzzyRule fr;
	float distNear = CFuzzyUtilities::Not(CFuzzyUtilities::Grade(tl, 0.0f, 2.5f));

	return static_cast<int>(distNear * 100.0f) > CUtilities::Random(100);
}

bool CCpu::AI_IsEnemyAttack()
{
	return (m_pTarget->IsAttack() && !m_bJump);
}

void CCpu::AI_Guard()
{
	m_MoveAngle = MOF_ToRadian(0.0f);
	m_bGuard = true;
}

void CCpu::AI_Attack()
{
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

void CCpu::AI_Move()
{
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
