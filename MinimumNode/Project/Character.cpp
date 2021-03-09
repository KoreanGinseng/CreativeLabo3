#include	"Character.h"

/**
 * �R���X�g���N�^
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
 * �f�X�g���N�^
 */
CCharacter::~CCharacter(){
}

/**
 * �ǂݍ���
 */
bool CCharacter::Load(void){
	// ���b�V���̓ǂݍ���
	if(!m_Mesh.Load("Chr_01_ion_mdl_01.mom"))
	{
		return false;
	}
	m_pMotion = m_Mesh.CreateMotionController();

	//���탁�b�V���̓ǂݍ���
	if (!m_WeaponMesh.Load("Wep_01_ion_mdl_01.mom"))
	{
		return false;
	}
	return true;
}

/**
 * ������
 */
void CCharacter::Initialize(void){
	m_Pos = Vector3(0.0f,0.0f,0.0f);
	m_pMotion->ChangeMotionByName("bse_wait_lp", 1.0f, TRUE);

	//�J�����̐ݒ�
	m_Camera.SetViewPort();
	m_Camera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 2000.0f);
	UpdateCamera();
}

/**
 * �X�V
 *
 * ����
 * [in]			pMesh			���胁�b�V��
 */
void CCharacter::Update(LPMeshContainer pMesh){
	//���G���Ԃ�����Ό��炷
	if (m_Invincible > 0.0f)
	{
		m_Invincible -= GAMEFRAMETIME;
	}
	if (m_bDamage)
	{
		//�_���[�W�X�V
		UpdateDamage();
	}
	else if (m_bAttack)
	{
		//�U���X�V
		UpdateAttack();
	}
	else if (m_bJump)
	{
		//�W�����v�X�V
		UpdateJump();
	}
	else if (m_bGuard)
	{
		m_pMotion->ChangeMotionByName("act_guard", 1.0f, TRUE, FALSE);
		m_bMove = false;
	}
	//�ړ��X�V
	UpdateMove();

	//�X�e�[�W�ƐڐG����
	CollisionStage(pMesh);

	//---------------------------------------------------
	//�ۑ�S�W�����v�U������̒ǉ�
	//		�W�����v�U�����͏d�̗͂������x�𔼕��ɂ���
	//---------------------------------------------------
	m_Gravity -= GRAVITY;

	//���W�X�V
	m_Pos.y += m_Gravity;
	if (!m_bStage)
	{
		m_Pos += m_Move;
	}

	//�A�j���[�V�����̎��Ԃ�i�߂�
	m_pMotion->AddTimer(GAMEFRAMETIME);

	//�J�����X�V
	UpdateCamera();
}

/**
 * �ړ��X�V
 */
void CCharacter::UpdateMove(void){
	if (m_bMove)
	{
		if (!m_bAttack && !m_bDamage && !m_bGuard && !m_bJump)
		{
			m_pMotion->ChangeMotionByName("mov_run_lp", 1.0f, TRUE, FALSE);
		}

		//�J�����̑O�����̃x�N�g��
		CVector3 cfvec = m_Camera.GetViewFront();
		//�J������Y���̉�]�p�x�����߂�
		float cy = atan2(cfvec.z, -cfvec.x) + MOF_MATH_HALFPI;
		//�ړ��p�x�����߂�
		float my = m_MoveAngle + cy;
		MOF_NORMALIZE_RADIANANGLE(my);
		//�����p�x
		float sa = my - m_Angle.y;
		MOF_ROTDIRECTION_RADIANANGLE(sa);
		//��]
		m_Angle.y += MOF_CLIPING(sa, -CHARACTER_ROTATIONSPEED, CHARACTER_ROTATIONSPEED);
		MOF_NORMALIZE_RADIANANGLE(m_Angle.y);

		//�ړ������̃x�N�g��
		CVector3 fvec(0, 0, -1);
		fvec.RotationY(my);
		m_Move += fvec * CHARACTER_MOVESPEED;
		//�ړ����ō����x�ŃN���b�v����
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
 * �U���J�n
 */
void CCharacter::StartAttack(){
	//�_���[�W���͓���J�n�ł��Ȃ�
	if (m_bDamage)
	{
		return;
	}
	//�W�����v�̓��ݍ��݂⒅�n���͓���J�n�ł��Ȃ�
	if (m_bJumpStart || m_bJumpEnd)
	{
		return;
	}
	//�U����Ԃ̐ݒ�
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
	//�ۑ�S�W�����v�U������̒ǉ�
	//		m_bJump�t���O���m�F���W�����v���Ȃ�A�d�͒l��0.1�ɐݒ�
	//		�Đ�����A�j���[�V���������L�ɕύX
	//		�U���P�i�ځFatk_jump_01
	//		�U���Q�i�ځFatk_jump_02
	//		�U���R�i�ځFatk_jump_03
	//---------------------------------------------------

	//�ʏ�U���̃A�j���[�V��������`
	static char* aname[] = {
		"atk_attack_01",
		"atk_attack_02",
		"atk_attack_03",
	};
	//�ʏ�U���̃A�j���[�V�����֕ύX
	m_pMotion->ChangeMotionByName(aname[m_AttackCount], 1.0f, FALSE, FALSE);
}

/**
 * �U���X�V
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
	//�U���̓A�j���[�V�����I���Ŏ��̓����
	if (m_pMotion->IsEndMotion())
	{
		//���̓��삪�o�^�ς�
		if (m_bNextAttack && m_AttackCount + 1 < CHARACTER_ATTACKCOUNT)
		{
			StartAttack();
		}
		//�U���̏I��
		else if (m_bAttackEnd)
		{
			m_bAttack = false;
		}
		else
		{
			//�I���A�j���[�V����
			m_bAttackEnd = true;

			//---------------------------------------------------
			//�ۑ�S�W�����v�U���I������̒ǉ�
			//		m_bJump�t���O���m�F���W�����v���Ȃ�
			//		�Đ�����A�j���[�V���������L�ɕύX
			//		�U���P�i�ځFatk_jump_01_ft
			//		�U���Q�i�ځFatk_jump_02_ft
			//		�U���R�i�ځFatk_jump_03_ft
			//---------------------------------------------------

			//�ʏ�U���̃A�j���[�V��������`
			static char* aname[] = {
				"atk_attack_01_ft",
				"atk_attack_02_ft",
				"atk_attack_03_ft",
			};
			//�ʏ�U���̃A�j���[�V�����֕ύX
			m_pMotion->ChangeMotionByName(aname[m_AttackCount], 1.0f, FALSE, FALSE);
		}
	}
}

/**
 * �_���[�W�J�n
 *
 * ����
 * [in]			chara			����L����
 */
void CCharacter::StartDamage(CCharacter& chara){
	//�ق��̓���͒��f
	m_bAttack = false;
	m_bNextAttack = false;
	m_bJump = false;
	//�U���̍ŏI�i�Ń_�E��������
	if (chara.m_AttackCount >= CHARACTER_ATTACKCOUNT - 1)
	{
		//�_���[�W�̊J�n�Ɩ��G���Ԃ̐ݒ�
		m_bDamage = true;
		m_bDown = true;
		m_Invincible = 1.5f;
		//����L��������̍U�������Ƀm�b�N�o�b�N
		m_Move = m_Pos - chara.m_Pos;
		m_Move.Normal(m_Move);
		m_Move *= 0.3f;
		m_Move.y += 0.3f;
		//�A�j���[�V�����ύX
		m_pMotion->ChangeMotionByName("dam_down_u", 1.0f, FALSE, TRUE);
	}
	else
	{
		//�_���[�W�̊J�n�Ɩ��G���Ԃ̐ݒ�
		m_bDamage = true;
		m_bDown = false;
		m_Invincible = 0.4f;
		static char* aname[] = {
			"dam_damage_m_a",
			"dam_damage_m_b",
			"dam_damage_m_c",
		};
		//����L��������̍U�������Ƀm�b�N�o�b�N
		m_Move = m_Pos - chara.m_Pos;
		m_Move.Normal(m_Move);
		m_Move *= 0.15f;
		//�A�j���[�V�����ύX
		m_pMotion->ChangeMotionByName(aname[CUtilities::Random(3)], 1.0f, FALSE, TRUE);
	}
}

/**
 * �_���[�W�X�V
 */
void CCharacter::UpdateDamage(){
	//���̑��̓�����L�����Z��
	m_bAttack = false;
	m_bGuard = false;
	m_bMove = false;
	m_bJump = false;
	m_bJumpEnd = false;
	m_bJumpStart = false;
	//�m�b�N�o�b�N�͏��X�Ɍ���
	m_Move *= 0.98f;
	if (m_bDown)
	{
		//�_�E���̓A�j���[�V�����I���Ń_���[�W��
		if (m_pMotion->IsEndMotion() && m_Invincible <= 0.0f)
		{
			m_bDown = false;
			m_Invincible = 1.0f;
			m_pMotion->ChangeMotionByName("dam_down_u_standup", 1.0f, FALSE, TRUE);
		}
	}
	else
	{
		//�_���[�W�̓A�j���[�V�����I���Ŏ��̓����
		if (m_pMotion->IsEndMotion())
		{
			m_bDamage = false;
		}
	}
}

/**
 * �W�����v�J�n
 */
void CCharacter::StartJump(){
	//�_���[�W����U�����̓W�����v�ł��Ȃ�
	if (m_bDamage || m_bAttack)
	{
		return;
	}

	//---------------------------------------------------
	//�ۑ�Q�W�����v�J�n�����̒ǉ�
	//		�W�����v�t���O�ƃW�����v�J�n�t���O��true
	//		�W�����v�I���t���O��false�ɂ���"mov_jump_f_set"�ɃA�j���[�V������ύX
	//---------------------------------------------------
}

/**
 * �W�����v�X�V
 */
void CCharacter::UpdateJump(){
	//---------------------------------------------------
	//�ۑ�Q�W�����v�����̒ǉ�
	//		�W�����v�J�n���F"mov_jump_f_set"�̃A�j���[�V�����I���ŁA�㏸�J�n
	//						�d�͂�0.3�ɐݒ�A�W�����v�J�n�t���O��false�ɂ��āA�A�j���[�V������"mov_jump_f_u_lp"�֕ύX
	//
	//		�W�����v�I�����F"mov_jump_landing"�̃A�j���[�V�����I����
	//						�W�����v�t���O�ƃW�����v�I���t���O��false�ɂ��āA�W�����v�I��
	//
	//		�W�����v���F	������ԁi�d�͂��[�j�ɂȂ����ꍇ�A
	//						�A�j���[�V������"mov_jump_f_d_lp"�֕ύX
	//---------------------------------------------------
}

/**
 * �J�����X�V
 */
void CCharacter::UpdateCamera(){
	//�J�����̐ݒ�
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
 * �X�e�[�W�̃��b�V���Ɠ����蔻��������Ȃ�
 *
 * ����
 * [in]			pMesh			���胁�b�V��
 */
void CCharacter::CollisionStage(LPMeshContainer pMesh){
	//���菉����
	m_bGround = false;
	m_bStage = false;
	//�ڐG���
	COLLISIONOUTGEOMETRY gout;
	//�i�s�����ւ̃��C
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
	//�������ւ̃��C
	CRay3D dray(m_Pos, CVector3(0, -1, 0));
	dray.Position.y += 2.0f;
	if(dray.CollisionMesh(pMesh, gout))
	{
		if (gout.d <= 2.1f)
		{
			m_Pos.y += 2.1f - gout.d;

			//---------------------------------------------------
			//�ۑ�Q�n�ʂւ̐ڐG�Œ��n���[�V���������s
			//		�W�����v�������ɒn�ʂɐڐG�����ꍇ�A
			//		�W�����v�I���t���O��true�ɂ��ăA�j���[�V������"mov_jump_landing"�֕ύX
			//---------------------------------------------------
			
			//�ʏ��Ԃł̒n�ʂւ̐ڐG����
			if (m_Gravity < 0)
			{
				m_Gravity = 0.0f;
				m_bGround = true;
			}
		}
	}
}

/**
 * �L�����N�^�[�Ɠ����蔻��������Ȃ�
 *
 * ����
 * [in]			chara			����L����
 */
void CCharacter::CollisionCharacter(CCharacter& chara){
	//���̂��擾���ē����蔻��
	CSphere p1 = GetSphere();
	CSphere p2 = chara.GetSphere();
	if (p1.CollisionSphere(p2))
	{
		//���܂�h�~�������Ȃ�
		CVector3 sv = p1.Position - p2.Position;
		float sl = (p1.r + p2.r) - sv.Length();
		sv.Normal(sv);
		m_Pos += sv * sl * 0.5f;
		chara.m_Pos -= sv * sl * 0.5f;
	}

	//���݂��̍U���̓����蔻��
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
 * �L�����N�^�[�̃K�[�h����������Ȃ�
 *
 * ����
 * [in]			chara			����L����
 */
bool CCharacter::CollisionGuard(CCharacter& chara){
	//�K�[�h���͐ڐG�������Ă��O������̍U���Ȃ疳���ɂ���
	if (!m_bGuard)
	{
		return false;
	}
	//����ւ̃x�N�g���Ǝ����̑O���x�N�g���̓��ς����i>�O�j�̏ꍇ�͉s�p�ł���O���X�O���ȓ��Ɣ��f�ł���
	CVector3 ev = chara.m_Pos - m_Pos;
	CVector3 fv(0, 0, -1);
	CMatrix33 matRot;
	matRot.RotationZXY(m_Angle);
	fv *= matRot;
	if (fv.Dot(ev) > 0.0f)
	{
		//�K�[�h����
		return true;
	}
	return false;
}

/**
 * �J������L���ɂ���
 */
void CCharacter::SetCameraEnable(){
	CGraphicsUtilities::SetCamera(&m_Camera);
}

/**
 * �`��
 */
void CCharacter::Render(void){
	// ���[���h�s��쐬
	CMatrix44 matWorld;
	matWorld.RotationZXY(m_Angle);
	matWorld.SetTranslation(m_Pos);
	//�A�j���[�V�����Ƀ��[���h�}�g���N�X��K�p
	m_pMotion->RefreshBoneMatrix(matWorld);
	// ���b�V���̕`��
	m_Mesh.Render(m_pMotion);

	//�����ݒ肷��{�[���̏����擾����
	LPBONEMOTIONSTATE pBoneState = m_pMotion->GetBoneState("UPP_weapon");
	if (!pBoneState)
	{
		return;
	}
	//���탁�b�V����`�悷��s����{�[����񂩂�v�Z����
	CMatrix44 matWeapon = pBoneState->pBone->GetRotationOffsetMatrix() * pBoneState->BoneMatrix;
	//���탁�b�V���̕`��
	m_WeaponMesh.Render(matWeapon);
}

/**
 * �f�o�b�O�`��
 */
void CCharacter::RenderDebug(void){
	// �����蔻��̕\��
	CGraphicsUtilities::RenderLineSphere(GetSphere(),Vector4(0,1,0,0.3f));
	// �U�������蔻��̕\��
	CGraphicsUtilities::RenderLineSphere(GetAttackSphere(), Vector4(1, 0, 0, 0.3f));
}

/**
 * �f�o�b�O�����`��
 */
void CCharacter::RenderDebugText(void){
	// �ʒu�̕`��
	CGraphicsUtilities::RenderString(10,40,MOF_COLOR_WHITE,
			"�v���C���[�ʒu X : %.1f , Y : %.1f , Z : %.1f",m_Pos.x,m_Pos.y,m_Pos.z);
}

/**
 * ���
 */
void CCharacter::Release(void){
	MOF_SAFE_DELETE(m_pMotion);
	m_Mesh.Release();
	m_WeaponMesh.Release();
}

/**
 * �U���̓����蔻��p���̎擾
 */
CSphere CCharacter::GetAttackSphere(){
	//�����ݒ肷��{�[���̏����擾����
	LPBONEMOTIONSTATE pBoneState = m_pMotion->GetBoneState("UPP_weapon");
	if (!pBoneState)
	{
		return CSphere(m_Pos, 0.5f);
	}
	//���탁�b�V����`�悷��s����{�[����񂩂�v�Z����
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