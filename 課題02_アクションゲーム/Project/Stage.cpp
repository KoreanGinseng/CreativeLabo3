#include	"Stage.h"

/**
* �R���X�g���N�^
*/
CStage::CStage() :
m_Mesh() ,
m_Light() ,
m_FarMesh() ,
m_SkyMesh() ,
m_Cloud1Mesh() ,
m_Cloud1Rotate(0.0f) ,
m_Cloud2Mesh() ,
m_Cloud2Rotate(0.0f) ,
m_Cloud3Mesh() ,
m_Cloud3Rotate(0.0f) ,
m_CollisionMesh() {
}

/**
* �f�X�g���N�^
*/
CStage::~CStage(){
}

/**
* �ǂݍ���
*/
bool CStage::Load(void){
	// ���b�V���̓ǂݍ���
	if (!m_Mesh.Load("Stage_01_mdl_base.mom"))
	{
		return false;
	}
	if (!m_FarMesh.Load("Stage_01_mdl_far.mom"))
	{
		return false;
	}
	if (!m_SkyMesh.Load("Stage_01_efc_mdl_01.mom"))
	{
		return false;
	}
	if (!m_Cloud1Mesh.Load("Stage_01_efc_mdl_02.mom"))
	{
		return false;
	}
	if (!m_Cloud2Mesh.Load("Stage_01_efc_mdl_03.mom"))
	{
		return false;
	}
	if (!m_Cloud3Mesh.Load("Stage_01_efc_mdl_06.mom"))
	{
		return false;
	}
	if (!m_CollisionMesh.Load("Stage_01_mdl_hit.mom"))
	{
		return false;
	}
	return true;
}

/**
* ������
*/
void CStage::Initialize(void){
	//�X�e�[�W�̌����̐ݒ�
	m_Light.SetDirection(Vector3(-1, -1, 1));
	m_Light.SetDiffuse(MOF_COLOR_WHITE);
	m_Light.SetAmbient(MOF_COLOR_HWHITE);
	CGraphicsUtilities::SetDirectionalLight(&m_Light);
}

/**
* �X�V
*/
void CStage::Update(void){
	m_Cloud1Rotate += 0.0002f;
	m_Cloud2Rotate += 0.0005f;
	m_Cloud3Rotate += 0.0010f;
}

/**
* �`��
*/
void CStage::Render(void){
	// ���[���h�s��쐬
	CMatrix44 matWorld;
	//�V�����b�V���̕`��
	m_SkyMesh.Render(matWorld);
	//�_���b�V�������ꂼ���]�����ĕ`��
	CMatrix44 matCloud1;
	matCloud1.RotationY(m_Cloud1Rotate);
	m_Cloud1Mesh.Render(matCloud1);
	CMatrix44 matCloud2;
	matCloud2.RotationY(m_Cloud2Rotate);
	m_Cloud2Mesh.Render(matCloud2);
	CMatrix44 matCloud3;
	matCloud3.RotationY(m_Cloud3Rotate);
	m_Cloud3Mesh.Render(matCloud3);
	//���i���b�V���̕`��
	m_FarMesh.Render(matWorld);
	// ���b�V���̕`��
	m_Mesh.Render(matWorld);
}

/**
* �f�o�b�O�����`��
*/
void CStage::RenderDebugText(void){
}

/**
* ���
*/
void CStage::Release(void){
	m_FarMesh.Release();
	m_SkyMesh.Release();
	m_Cloud1Mesh.Release();
	m_Cloud2Mesh.Release();
	m_Cloud3Mesh.Release();
	m_Mesh.Release();
	m_CollisionMesh.Release();
}