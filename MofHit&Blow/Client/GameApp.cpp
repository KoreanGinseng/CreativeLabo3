/*************************************************************************//*!

					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"

#include    "Title.h"
#include    "MatchWait.h"
#include    "PlaySetting.h"
#include    "ServerSelect.h"
#include    "Game.h"

#include    "ResourceManager.h"

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {

    if(!CResourceManager::Load())
    {
        return FALSE;
    }

    m_SceneManager
        .Add<CTitle       >(SceneName::Title)
        .Add<CMatchWait   >(SceneName::MatchWait)
        .Add<CPlaySetting >(SceneName::PlaySetting)
        .Add<CServerSelect>(SceneName::ServerSelect)
        .Add<CGame        >(SceneName::Game)
        .SetFadeColor(MOF_COLOR_WHITE);

    m_SceneManager.Initialize(SceneName::PlaySetting);

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void) {
	//キーの更新
	g_pInput->RefreshKey();

    if (!m_SceneManager.Update())
    {
        return FALSE;
    }

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void) {
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    if (!m_SceneManager.Render())
    {
        return FALSE;
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
MofBool CGameApp::Release(void) {

    CResourceManager::Release();

	return TRUE;
}