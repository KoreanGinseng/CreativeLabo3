#include	<stdio.h>

//�\�P�b�g�ʐM�p
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//�X���b�h�p
#include	<process.h>
#include	<windows.h>

//�N���C�A���g�\����
typedef struct tag_CLIENT {
	BOOL		bConnect;
	SOCKET		Socket;
}CLIENT;
#define			MAXCLIENT			10
CLIENT			Client[MAXCLIENT];

SOCKET			AcceptSock;

unsigned int __stdcall RecvThread(void* pData){
	CLIENT* pClient = (CLIENT*)pData;
	pClient->bConnect = TRUE;
	while(TRUE)
	{
		int Data;
		int s = recv(pClient->Socket,(char*)&Data,sizeof(int),0);
		printf("%d Bytes Receive\n",s);
		printf("%d Receive\n",Data);
		if(Data == -1)
		{
			printf("-1 Receive End\n");
			break;
		}
		else if(s <= 0)
		{
			printf("-1 Connect End\n");
			break;
		}
	}
	pClient->bConnect = FALSE;
	_endthreadex(NULL);
	return 0;
}

unsigned int __stdcall AcceptThread(void* pData){
	//�\�P�b�g�̍쐬
	AcceptSock = socket(AF_INET, SOCK_STREAM, 0);
	if(AcceptSock == INVALID_SOCKET)
	{
		printf("Error SockCreate\n");
		return 0;
	}
	//�A�h���X�\����
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr,0,sizeof(struct sockaddr_in));
	//�l�b�g���[�N�̃f�[�^��ݒ�
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);
	tmp_addr.sin_addr.s_addr = ADDR_ANY;
	//�A�h���X�ƃ\�P�b�g���o�C���h
	if((bind(AcceptSock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		printf("Error SockBind\n");
		closesocket(AcceptSock);
		return 0;
	}
	//�ڑ��ҋ@��Ԃɂ���
	if((listen(AcceptSock,SOMAXCONN)) == SOCKET_ERROR)
	{
		printf("Error SockListen\n");
		closesocket(AcceptSock);
		return 0;
	}

	while(TRUE)
	{
		//�ڑ����Ă����A�h���X���
		SOCKADDR_IN ta;
		//�A�h���X�\���̂̃T�C�Y
		int addrin_size = sizeof(SOCKADDR_IN);
		//�ڑ��҂�
		printf("Wait Accept\n");
		SOCKET ts = accept(AcceptSock,(struct sockaddr*)&ta,&addrin_size);
		if(ts == INVALID_SOCKET)
		{
			break;
		}
		for(int i = 0;i < MAXCLIENT;i++)
		{
			if(Client[i].bConnect)
			{
				continue;
			}
			printf("Client IP %s\n",inet_ntoa(ta.sin_addr));
			//��M�X���b�h�̊J�n
			Client[i].Socket = ts;
			HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,RecvThread,&Client[i],0,NULL);
			break;
		}
	}
	_endthreadex(NULL);
	return 0;
}

void main(void)
{
	//WSAStartUp
	WSAData wsaData;
	WSAStartup(MAKEWORD(2,0),&wsaData);

	//�N���C�A���g�̏���������
	memset(Client,0,sizeof(Client));
	//�ڑ��ҋ@�X���b�h�̊J�n
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,AcceptThread,"Thread1",0,NULL);

	//��M
	while(TRUE)
	{
		int Data;
		printf("Press Send INT Data : ");
		scanf("%d",&Data);
		if(Data == -1)
		{
			printf("-1 Send End\n");
			break;
		}
		for(int i = 0;i < MAXCLIENT;i++)
		{
			if(!Client[i].bConnect)
			{
				continue;
			}
			int s = send(Client[i].Socket,(char*)&Data,sizeof(int),0);
			printf("%d Bytes Send\n",s);
			printf("%d Send\n",Data);
		}
	}
	//��n��
	closesocket(AcceptSock);
	for(int i = 0;i < MAXCLIENT;i++)
	{
		if(!Client[i].bConnect)
		{
			continue;
		}
		closesocket(Client[i].Socket);
	}
	//�X���b�h�̒�~�ҋ@
	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread1);
	WSACleanup();
	return;
}