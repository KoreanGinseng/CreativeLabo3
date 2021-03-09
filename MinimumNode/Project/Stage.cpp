#include	"Stage.h"

/**
* コンストラクタ
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
* デストラクタ
*/
CStage::~CStage(){
}

/**
* 読み込み
*/
bool CStage::Load(void){
	// メッシュの読み込み
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
* 初期化
*/
void CStage::Initialize(void){
	//ステージの光源の設定
	m_Light.SetDirection(Vector3(-1, -1, 1));
	m_Light.SetDiffuse(MOF_COLOR_WHITE);
	m_Light.SetAmbient(MOF_COLOR_HWHITE);
	CGraphicsUtilities::SetDirectionalLight(&m_Light);
}

/**
* 更新
*/
void CStage::Update(void){
	m_Cloud1Rotate += 0.0002f;
	m_Cloud2Rotate += 0.0005f;
	m_Cloud3Rotate += 0.0010f;
}

/**
* 描画
*/
void CStage::Render(void){
	// ワールド行列作成
	CMatrix44 matWorld;
	//天球メッシュの描画
	m_SkyMesh.Render(matWorld);
	//雲メッシュをそれぞれ回転させて描画
	CMatrix44 matCloud1;
	matCloud1.RotationY(m_Cloud1Rotate);
	m_Cloud1Mesh.Render(matCloud1);
	CMatrix44 matCloud2;
	matCloud2.RotationY(m_Cloud2Rotate);
	m_Cloud2Mesh.Render(matCloud2);
	CMatrix44 matCloud3;
	matCloud3.RotationY(m_Cloud3Rotate);
	m_Cloud3Mesh.Render(matCloud3);
	//遠景メッシュの描画
	m_FarMesh.Render(matWorld);
	// メッシュの描画
	m_Mesh.Render(matWorld);
}

/**
* デバッグ文字描画
*/
void CStage::RenderDebugText(void){
}

/**
* 解放
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