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

//データ送信用構造体
struct ReceiveData {
	//データ番豪
	MofU32			no;
	//座標
	float			x;
	float			y;
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
		//キー操作で移動
		if (g_pInput->IsKeyHold(MOFKEY_LEFT))
		{
			g_mx -= 1.0f;
		}
		else if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
		{
			g_mx += 1.0f;
		}
		if (g_pInput->IsKeyHold(MOFKEY_UP))
		{
			g_my -= 1.0f;
		}
		else if (g_pInput->IsKeyHold(MOFKEY_DOWN))
		{
			g_my += 1.0f;
		}
		//相手に座標データを送信する
		ReceiveData msg;
		msg.no = g_SNo++;						//送信データへの格納後に送信番号を進める
		msg.x = g_mx;
		msg.y = g_my;
		//ＩＰアドレス（送信先のＩＰを入力すること）
		MofU32 ip = inet_addr("127.0.0.1");
		g_SendSock.Send(ip, g_SPort, &msg, sizeof(msg));
		//受信ソケットにたまっている情報を受信する
		while (g_ReceiveSock.IsReceive())
		{
			//実際に受信をおこない相手の座標に設定する
			MOFSOCKETADDR addr;
			if (g_ReceiveSock.Receive(&msg, sizeof(msg), addr) == sizeof(msg))
			{
				//自分の受信番号より古いデータでは座標も番号も更新しない
				if (msg.no > g_RNo)
				{
					g_tx = msg.x;
					g_ty = msg.y;
					g_RNo = msg.no;
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