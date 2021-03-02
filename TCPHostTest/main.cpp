#include	<stdio.h>

//�\�P�b�g�ʐM�p
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//�X���b�h�p
#include	<process.h>
#include	<windows.h>

//�\�P�b�g���X���b�h�ł��g�����߃O���[�o��
SOCKET ts;

unsigned int __stdcall Thread(void* pData){
	while(TRUE)
	{
		int Data;
		int s = recv(ts,(char*)&Data,sizeof(int),0);
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
	SOCKET tsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tsock == INVALID_SOCKET)
	{
		printf("Error SockCreate\n");
		return;
	}
	//�A�h���X�\����
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr,0,sizeof(struct sockaddr_in));
	//�l�b�g���[�N�̃f�[�^��ݒ�
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);
	tmp_addr.sin_addr.s_addr = ADDR_ANY;
	//�A�h���X�ƃ\�P�b�g���o�C���h
	if((bind(tsock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		printf("Error SockBind\n");
		closesocket(tsock);
		return;
	}
	//�ڑ��ҋ@��Ԃɂ���
	if((listen(tsock,SOMAXCONN)) == SOCKET_ERROR)
	{
		printf("Error SockListen\n");
		closesocket(tsock);
		return;
	}
	//�ڑ����Ă����A�h���X���
	SOCKADDR_IN ta;
	//�A�h���X�\���̂̃T�C�Y
	int addrin_size = sizeof(SOCKADDR_IN);
	//�ڑ��҂�
	printf("Wait Accept\n");
	ts = accept(tsock,(struct sockaddr*)&ta,&addrin_size);
	printf("Client IP %s\n",inet_ntoa(ta.sin_addr));
	//��M�X���b�h�̊J�n
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,Thread,"Thread1",0,NULL);
	//�\�P�b�g�̃u���b�L���O����������
	u_long val = 1;
	ioctlsocket(ts, FIONBIO, &val);
	//��M
	while(TRUE)
	{
		int Data;
		printf("Press Send INT Data : ");
		scanf("%d",&Data);
		int s = send(ts,(char*)&Data,sizeof(int),0);
		printf("%d Bytes Send\n",s);
		printf("%d Send\n",Data);
		if(Data == -1)
		{
			printf("-1 Send End\n");
			break;
		}
	}
	//�X���b�h�̒�~�ҋ@
	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread1);
	//��n��
	closesocket(ts);
	closesocket(tsock);
	WSACleanup();
	return;
}