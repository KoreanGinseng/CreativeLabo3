/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	<vector>

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

//�f�[�^�w�b�_�[
struct ReceiveHeader{
	//���b�Z�[�W�^�C�v
	int				type;
	//�f�[�^�ԍ�
	MofU32			no;
	//���b�Z�[�W���M����
	MofU32			time;
};

//���M�^�C�v
enum MsgType {
	TYPE_KEY,					//�L�[����
	TYPE_REP,					//�ԐM
};
//�f�[�^���M�p�\���́i���W�j
struct ReceiveRep : public ReceiveHeader{
	//�R���X�g���N�^
	//�����Ńw�b�_�[�̃^�C�v���w�肷��悤��
	//�쐬����Ίy
	ReceiveRep(){
		type = TYPE_REP;
	}
};
//�L�[�^�C�v
enum MsgKey {
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN,
};
//�f�[�^���M�p�\���́i�L�[�j
struct ReceiveKey : public ReceiveHeader{
	//�L�[
	MofU32			key;

	//�R���X�g���N�^
	//�����Ńw�b�_�[�̃^�C�v���w�肷��悤��
	//�쐬����Ίy
	ReceiveKey(){
		type = TYPE_KEY;
	}
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

class FindFunc {
public:
	int n;
	FindFunc(int _n) :n(_n){}
	bool operator()(const ReceiveKey& a) const { return a.no == n; }
};


//���M���b�Z�[�W
std::vector< ReceiveKey >		g_SendKey;
//��M���b�Z�[�W
std::vector< ReceiveKey >		g_ReceiveKey;
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
		//�L�[���͂𑗐M����
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
		//�h�o�A�h���X�i���M��̂h�o����͂��邱�Ɓj
		MofU32 ip = inet_addr("127.0.0.1");
		g_SendSock.Send(ip, g_SPort, &key, sizeof(key));
		//���b�Z�[�W�̑��M���Ԃ��L�^���đ��M���b�Z�[�W�Ƃ��ĕۑ�����
		MofU32 nt = timeGetTime();
		key.time = nt;
		g_SendKey.push_back(key);
		//���M���b�Z�[�W�����莞�ԕԐM�̂Ȃ����̂��đ�����
		for (auto it = g_SendKey.begin(); it != g_SendKey.end(); ++it)
		{
			if (nt - it->time >= 80)
			{
				key = *it;
				g_SendSock.Send(ip, g_SPort, &key, sizeof(key));
				it->time = nt;
			}
		}

		//��M�\�P�b�g�ɂ��܂��Ă��������M����
		while (g_ReceiveSock.IsReceive())
		{
			//�S���b�Z�[�W����M�ł��邾���̃T�C�Y�����o�b�t�@
			//�T�C�Y�̓��b�Z�[�W�̍ő�T�C�Y���l�����Č��߂邱��
			char buf[32];
			MOFSOCKETADDR addr;
			if (g_ReceiveSock.Receive(buf, sizeof(buf), addr) > 0)
			{
				//�܂��w�b�_�[�Ŏ��o���ă^�C�v�ɂ���ď�����ς���
				ReceiveHeader* pHeader = (ReceiveHeader*)buf;
				//�ԐM���b�Z�[�W�̏ꍇ�ԐM�̂��������M���b�Z�[�W���폜
				if (pHeader->type == TYPE_REP)
				{
					//���M�ς݃L�[�ɂ���΍폜
					g_SendKey.erase(remove_if(g_SendKey.begin(), g_SendKey.end(), FindFunc(pHeader->no)), g_SendKey.end());
				}
				else
				{
					//��M����ƕK���ԐM�������Ȃ�
					ReceiveRep rep;
					rep.no = pHeader->no;
					rep.time = pHeader->time;
					g_SendSock.Send(ip, g_SPort, &rep, sizeof(rep));
					//�Â����b�Z�[�W�ł͏������Ȃ�
					if (g_RNo > pHeader->no)
					{
						continue;
					}
					//���Ɏ�M�ς݂̃��b�Z�[�W���������Ȃ�
					auto fit = std::find_if(g_ReceiveKey.begin(), g_ReceiveKey.end(), FindFunc(pHeader->no));
					if (fit != g_ReceiveKey.end())
					{
						continue;
					}
					//�L�[�ɒǉ����Ĕԍ����Ƀ\�[�g
					ReceiveKey* msg = (ReceiveKey*)buf;
					g_ReceiveKey.push_back(*msg);
					sort(g_ReceiveKey.begin(), g_ReceiveKey.end(), [](const ReceiveKey& x, const ReceiveKey& y) { return x.no < y.no; });
					while (g_ReceiveKey.size() > 0 && g_RNo == g_ReceiveKey.at(0).no)
					{
						g_RNo++;
						key = g_ReceiveKey.at(0);
						//����̃L�[�ňړ�
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
						g_ReceiveKey.erase(g_ReceiveKey.begin());
					}
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