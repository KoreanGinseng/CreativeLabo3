/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"Player.h"
#include	"Cpu.h"
#include	"Stage.h"

//キャラクタの宣言
CPlayer					gPlayer;
CCpu					gCpu;
//ステージの宣言
CStage					gStage;
// デバッグ表示フラグ
bool					gbDebug = false;
// カメラ操作フラグ
bool					gbCamera = false;

//カメラ
CCamera					gCamera;
//カメラ位置
CVector3				gCPos(0, 0, -5);
//カメラ注視点位置
CVector3				gTPos(0, 0, 0);
//カメラの上方ベクトル
CVector3				gCUp(0, 1, 0);
/**
 * カメラ更新
 */
void UpdateCamera(){
	if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		CVector3 vv = gCPos - gTPos;
		vv.RotationY(0.01f);
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		CVector3 vv = gCPos - gTPos;
		vv.RotationY(-0.01f);
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	if (g_pInput->IsKeyHold(MOFKEY_UP))
	{
		CVector3 vv = gCPos - gTPos;
		float d = sqrt(vv.x * vv.x + vv.z * vv.z);
		MofFloat sina = MOF_SIN(-0.01f);
		MofFloat cosa = MOF_COS(-0.01f);
		MofFloat tmpy = vv.y * cosa + d * -sina;
		MofFloat tmpxz = vv.y * sina + d * cosa;
		vv.y = tmpy;
		vv.x = (vv.x / d) * tmpxz;
		vv.z = (vv.z / d) * tmpxz;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	else if (g_pInput->IsKeyHold(MOFKEY_DOWN))
	{
		CVector3 vv = gCPos - gTPos;
		float d = sqrt(vv.x * vv.x + vv.z * vv.z);
		MofFloat sina = MOF_SIN(0.01f);
		MofFloat cosa = MOF_COS(0.01f);
		MofFloat tmpy = vv.y * cosa + d * -sina;
		MofFloat tmpxz = vv.y * sina + d * cosa;
		vv.y = tmpy;
		vv.x = (vv.x / d) * tmpxz;
		vv.z = (vv.z / d) * tmpxz;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}

	//マウス入力でカメラ操作
	CVector3 mv;
	g_pInput->GetMouseMove(mv);
	//ホイールによるカメラのズーム調整
	if (mv.z != 0)
	{
		CVector3 vv = gCPos - gTPos;
		vv *= 1.0f - mv.z * 0.0001f;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	else if (g_pInput->IsMouseKeyHold(2))
	{
		CVector3 vv = gCPos - gTPos;
		vv *= 1.0f + mv.y * 0.01f;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	//左クリックによるカメラ移動
	if (g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON))
	{
		CVector3 sv;
		CVector3 uv;
		CVector3 vv = gTPos - gCPos;
		vv.Normal(vv);
		vv.Cross(gCUp, sv);
		sv.Normal(sv);
		gCPos += sv * mv.x * 0.01f;
		gTPos += sv * mv.x * 0.01f;
		sv.Cross(vv, uv);
		uv.Normal(uv);
		gCPos += uv * mv.y * 0.01f;
		gTPos += uv * mv.y * 0.01f;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
	//右クリックによるカメラ回転
	if (g_pInput->IsMouseKeyHold(MOFMOUSE_RBUTTON))
	{
		CVector3 vv = gCPos - gTPos;
		vv.RotationY(mv.x * 0.01f);
		float d = sqrt(vv.x * vv.x + vv.z * vv.z);
		MofFloat sina = MOF_SIN(-mv.y * 0.01f);
		MofFloat cosa = MOF_COS(-mv.y * 0.01f);
		MofFloat tmpy = vv.y * cosa + d * -sina;
		MofFloat tmpxz = vv.y * sina + d * cosa;
		vv.y = tmpy;
		vv.x = (vv.x / d) * tmpxz;
		vv.z = (vv.z / d) * tmpxz;
		gCPos = gTPos + vv;
		gCamera.LookAt(gCPos, gTPos, Vector3(0, 1, 0));
		gCamera.Update();
	}
}

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");

	//カメラの設定
	gCamera.SetViewPort();
	gCamera.LookAt(gCPos, gTPos, gCUp);
	gCamera.PerspectiveFov(MOF_ToRadian(60.0f), 1024.0f / 768.0f, 0.01f, 2000.0f);
	gCamera.Update();
	CGraphicsUtilities::SetCamera(&gCamera);

	//キャラクタの読み込み
	CUtilities::SetCurrentDirectory("Chara");
	gPlayer.Load();
	gCpu.Load();
	CUtilities::SetCurrentDirectory("../");
	//ステージの読み込み
	CUtilities::SetCurrentDirectory("Stage");
	gStage.Load();
	CUtilities::SetCurrentDirectory("../");

	//初期化
	gPlayer.Initialize();
	gCpu.Initialize();
	gStage.Initialize();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();

	//キー入力でカメラ操作
	if (gbCamera)
	{
		UpdateCamera();
	}
	else
	{
		//キャラクタの更新
		gPlayer.Update(gCpu, gStage.GetCollisionMesh());
		gCpu.Update(gPlayer,gStage.GetCollisionMesh());
		gPlayer.CollisionCharacter(gCpu);
	}
	//ステージの更新
	gStage.Update();
	if (g_pInput->IsKeyPush(MOFKEY_RETURN))
	{
		gbCamera = ((gbCamera) ? false : true);
	}

	// デバッグ表示の切り替え
	if (g_pInput->IsKeyPush(MOFKEY_F1))
	{
		gbDebug = ((gbDebug) ? false : true);
	}
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f,0.0f,1.0f,0.0f,1.0f,0);

	//深度バッファを有効にする
	g_pGraphics->SetDepthEnable(TRUE);
	if (gbCamera)
	{
		CGraphicsUtilities::SetCamera(&gCamera);
	}
	else
	{
		//プレイヤーのカメラを有効にする
		gPlayer.SetCameraEnable();
	}

	//ステージの描画
	gStage.Render();
	//キャラクタの描画
	gPlayer.Render();
	gCpu.Render();
	//グリッドを描画する
	if (gbDebug)
	{
		CGraphicsUtilities::RenderGrid(2, 100, MOF_COLOR_WHITE, PLANEAXIS_ALL);
		gPlayer.RenderDebug();
		gCpu.RenderDebug();
	}

	// 深度バッファ無効化
	g_pGraphics->SetDepthEnable(FALSE);

	// 2Dデバッグ描画
	if (gbDebug)
	{
		//FPS描画
		CGraphicsUtilities::RenderString(10, 10, MOF_COLOR_WHITE, "%d", CUtilities::GetFPS());
		// プレイヤーのデバッグ文字描画
		gPlayer.RenderDebugText();
	}

	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){
	//ステージの解放
	gStage.Release();
	//キャラクタの解放
	gPlayer.Release();
	gCpu.Release();
	return TRUE;
}