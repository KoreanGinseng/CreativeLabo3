/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"

//���M�p�\�P�b�g
CUdpSocket			g_SendSock;
//��M�p�\�P�b�g
CUdpSocket			g_ReceiveSock;

//�ڑ��t���O
bool				g_bConnect = false;
//��M�|�[�g
MofU16				g_Port = 0;
//���M�|�[�g
MofU16				g_SPort = 0;

//�f�[�^���M�p�\����
struct ReceiveData {
	//�f�[�^�ԍ�
	MofU32			no;
	//���W
	float			x;
	float			y;
};

//�������W
float				g_mx = 0;
float				g_my = 0;
//���M�ԍ�
MofU32				g_SNo = 0;

//������W
float				g_tx = 0;
float				g_ty = 0;
//��M�ԍ�
MofU32				g_RNo = 0;
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//���M�\�P�b�g�̍쐬
	g_SendSock.Initialize();
	//��M�\�P�b�g�̍쐬
	g_ReceiveSock.Initialize();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//�L�[�̍X�V
	g_pInput->RefreshKey();
	
	if (g_bConnect)
	{
		//�L�[����ňړ�
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
		//����ɍ��W�f�[�^�𑗐M����
		ReceiveData msg;
		msg.no = g_SNo++;						//���M�f�[�^�ւ̊i�[��ɑ��M�ԍ���i�߂�
		msg.x = g_mx;
		msg.y = g_my;
		//�h�o�A�h���X�i���M��̂h�o����͂��邱�Ɓj
		MofU32 ip = inet_addr("127.0.0.1");
		g_SendSock.Send(ip, g_SPort, &msg, sizeof(msg));
		//��M�\�P�b�g�ɂ��܂��Ă��������M����
		while (g_ReceiveSock.IsReceive())
		{
			//���ۂɎ�M�������Ȃ�����̍��W�ɐݒ肷��
			MOFSOCKETADDR addr;
			if (g_ReceiveSock.Receive(&msg, sizeof(msg), addr) == sizeof(msg))
			{
				//�����̎�M�ԍ����Â��f�[�^�ł͍��W���ԍ����X�V���Ȃ�
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
		//1 or 2�Ń|�[�g�ԍ���ς��ăo�C���h
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
		@brief			�A�v���P�[�V�����̕`��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//�`�揈��
	g_pGraphics->RenderStart();
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f,0.0f,0.0f,0.0f,1.0f,0);

	if (g_bConnect)
	{
		//�����̈ʒu�ɐԁ���`��
		CGraphicsUtilities::RenderCircle(g_mx, g_my, 5.0f, MOF_COLOR_RED);

		//����̈ʒu�ɐ���`��
		CGraphicsUtilities::RenderCircle(g_tx, g_ty, 5.0f, MOF_COLOR_BLUE);
	}
	else
	{
		CGraphicsUtilities::RenderString(0, 0, MOF_COLOR_WHITE, "1�L�[��19001�Ԃ�bind�A2�L�[��19002�Ԃ�bind");
	}
	//�`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None
				
		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void){
	//���M�\�P�b�g�̐ؒf
	g_SendSock.Close();
	//��M�\�P�b�g�̐ؒf
	g_ReceiveSock.Close();
	return TRUE;
}