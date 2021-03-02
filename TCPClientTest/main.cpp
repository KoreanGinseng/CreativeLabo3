#include	<stdio.h>

//�\�P�b�g�ʐM�p
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//�X���b�h�p
#include	<process.h>
#include	<windows.h>

//�\�P�b�g���X���b�h�ł��g�����߃O���[�o��
SOCKET tsock;

unsigned int __stdcall Thread(void* pData){
	while(TRUE)
	{
		int Data;
		int s = recv(tsock,(char*)&Data,sizeof(int),0);
		printf("%d Bytes Receive\n",s);
		printf("%d Receive\n",Data);
		if(Data == -1)
		{
			printf("-1 Receive End\n");
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

	//�\�P�b�g�̍쐬
	tsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tsock == INVALID_SOCKET)
	{
		printf("Error SockCreate\n");
		return;
	}
	//�ڑ�IP�����
	char ip[256];
	printf("Press Connection IP : ");
	scanf("%s",ip);
	//�A�h���X�\����
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr,0,sizeof(struct sockaddr_in));
	//�l�b�g���[�N�̃f�[�^��ݒ�
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);
	tmp_addr.sin_addr.s_addr = inet_addr(ip);
	if(connect(tsock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		closesocket(tsock);
		printf("Error Connect\n");
		return;
	}
	//��M�X���b�h�̊J�n
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,Thread,"Thread1",0,NULL);
	//�\�P�b�g�̃u���b�L���O����������
	u_long val = 1;
	ioctlsocket(tsock, FIONBIO, &val);
	//���M
	while(TRUE)
	{
		char Data[1024];
		printf("Press Send CHAR Data : ");
		scanf("%s", Data);
		int s = send(tsock,(char*)Data,sizeof(Data),0);
		printf("%d Bytes Send\n",s);
		printf("%d Send\n",Data);
		if(s < 0)
		{
			printf("-1 Send End\n");
			break;
		}
	}
	//�X���b�h�̒�~�ҋ@
	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread1);
	//��n��
	closesocket(tsock);
	WSACleanup();
	return;
}