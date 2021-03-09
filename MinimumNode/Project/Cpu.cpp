#include	"Cpu.h"

/**
 * �R���X�g���N�^
 */
CCpu::CCpu() :
CCharacter() ,
m_CpuTime(0) {
}

/**
 * �f�X�g���N�^
 */
CCpu::~CCpu(){
	Release();
}

/**
* ������
*/
void CCpu::Initialize(void){
	m_Pos = Vector3(0.0f, 0.0f, -20.0f);
	m_Angle = Vector3(0.0f, MOF_MATH_PI, 0.0f);
	m_pMotion->ChangeMotionByName("bse_wait_lp", 1.0f, TRUE);

	//�J�����̐ݒ�
	m_Camera.SetViewPort();
	m_Camera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 2000.0f);
	UpdateCamera();

	//���s����֐��̓o�^
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
 * �X�V
 *
 * ����
 * [in]			chara			�^�[�Q�b�g�L����
 * [in]			pMesh			���胁�b�V��
 */
void CCpu::Update(CCharacter& chara,LPMeshContainer pMesh){
	//�^�[�Q�b�g�o�^
	m_pTarget = &chara;
	//�ΏۃL�����ւ̃x�N�g�����v�Z����
	CVector3 tv = m_pTarget->GetPosition() - m_Pos;
	float tl = tv.Length();
	//�J�����͏�ɑ���̕����Ɍ�����
	m_CameraAngle = atan2(tv.z, -tv.x) + MOF_MATH_HALFPI;
	//��莞�Ԃ��Ƃɓ���X�V
	m_CpuTime -= GAMEFRAMETIME;
	if (m_CpuTime <= 0)
	{
		root->Exec(this);
		//�v�l���Ԃ��Đݒ�
		m_CpuTime = 0.1f + tl / 20.0f;
	}
	if (!m_bAttack && !m_bGuard)
	{
		m_bMove = true;
	}
	CCharacter::Update(pMesh);
}

bool CCpu::CpuDistance() {
	//�ΏۃL�����ւ̃x�N�g�����v�Z����
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
		//���̓o�^
		m_bNextAttack = true;
	}
}
void CCpu::CpuMove() {
	//�����Ŏ΂߂̓��͂����肷��
	int rm = CUtilities::Random(3);
	if (rm == 0)
	{
		//����̕����ɑO�i����
		m_MoveAngle = MOF_ToRadian(0.0f);
	}
	else if (rm == 1)
	{
		//�E�O�������Ɉړ�����
		m_MoveAngle = MOF_ToRadian(45.0f);
	}
	else if (rm == 2)
	{
		//���O�������Ɉړ�����
		m_MoveAngle = MOF_ToRadian(315.0f);
	}
	m_bGuard = false;
}