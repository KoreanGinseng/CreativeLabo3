/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"

//送信用ソケット
CUdpSocket			g_SendSock;
//受信用ソケット
CUdpSocket			g_ReceiveSock;

//接続フラグ
bool				g_bConnect = false;
//受信ポート
MofU16				g_Port = 0;
//送信ポート
MofU16				g_SPort = 0;

//キータイプ
enum MsgKey {
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
};
//データ送信用構造体（キー）
struct ReceiveKey {
	//データ番豪
	MofU32			no;
	//キー
	MofU32			key;
};

//自分座標
float				g_mx = 0;
float				g_my = 0;
//送信番号
MofU32				g_SNo = 0;

//相手座標
float				g_tx = 0;
float				g_ty = 0;
//受信番号
MofU32				g_RNo = 0;
/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//送信ソケットの作成
	g_SendSock.Initialize();
	//受信ソケットの作成
	g_ReceiveSock.Initialize();
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
	
	if (g_bConnect)
	{
		//キー入力を送信する
		ReceiveKey key;
		key.no = g_SNo++;
		key.key = 0;
		if (g_pInput->IsKeyHold(MOFKEY_LEFT))
		{
			g_mx -= 1.0f;
			key.key |= 1 << KEY_LEFT;
		}
		else if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
		{
			g_mx += 1.0f;
			key.key |= 1 << KEY_RIGHT;
		}
		if (g_pInput->IsKeyHold(MOFKEY_UP))
		{
			g_my -= 1.0f;
			key.key |= 1 << KEY_UP;
		}
		else if (g_pInput->IsKeyHold(MOFKEY_DOWN))
		{
			g_my += 1.0f;
			key.key |= 1 << KEY_DOWN;
		}
		//ＩＰアドレス（送信先のＩＰを入力すること）
		MofU32 ip = inet_addr("127.0.0.1");
		g_SendSock.Send(ip, g_SPort, &key, sizeof(key));

		//受信ソケットにたまっている情報を受信する
		while (g_ReceiveSock.IsReceive())
		{
			MOFSOCKETADDR addr;
			if (g_ReceiveSock.Receive(&key, sizeof(key), addr) == sizeof(key))
			{
				//自分の受信番号より古いデータでは座標も番号も更新しない
				if (key.no > g_RNo)
				{
					//相手のキーで移動
					if (key.key & (1 << KEY_LEFT))
					{
						g_tx -= 1.0f;
					}
					else if (key.key & (1 << KEY_RIGHT))
					{
						g_tx += 1.0f;
					}
					if (key.key & (1 << KEY_UP))
					{
						g_ty -= 1.0f;
					}
					else if (key.key & (1 << KEY_DOWN))
					{
						g_ty += 1.0f;
					}
				}
			}
		}
	}
	else
	{
		//1 or 2でポート番号を変えてバインド
		if (g_pInput->IsKeyHold(MOFKEY_1))
		{
			g_Port = 19001;
			g_SPort = 19002;
			if (g_ReceiveSock.Bind(g_Port))
			{
				g_bConnect = true;
			}
		}
		else if (g_pInput->IsKeyHold(MOFKEY_2))
		{
			g_Port = 19002;
			g_SPort = 19001;
			if (g_ReceiveSock.Bind(g_Port))
			{
				g_bConnect = true;
			}
		}
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
	//描画処理
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f,0.0f,0.0f,0.0f,1.0f,0);

	if (g_bConnect)
	{
		//自分の位置に赤○を描画
		CGraphicsUtilities::RenderCircle(g_mx, g_my, 5.0f, MOF_COLOR_RED);

		//相手の位置に青○を描画
		CGraphicsUtilities::RenderCircle(g_tx, g_ty, 5.0f, MOF_COLOR_BLUE);
	}
	else
	{
		CGraphicsUtilities::RenderString(0, 0, MOF_COLOR_WHITE, "1キーで19001番でbind、2キーで19002番でbind");
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
	//送信ソケットの切断
	g_SendSock.Close();
	//受信ソケットの切断
	g_ReceiveSock.Close();
	return TRUE;
}