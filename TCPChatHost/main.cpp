#define		_WINSOCK_DEPRECATED_NO_WARNINGS
#define		_CRT_SECURE_NO_WARNINGS

#include	<stdio.h>

//�\�P�b�g�ʐM�p
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

int main(void)
{
	//WSAStartUp
	WSAData wsaData;
	int ret = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(ret != 0)
	{
		printf("WinSock�̏������Ɏ��s\n");
	}

	//�\�P�b�g�̍쐬
	SOCKET tsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tsock == INVALID_SOCKET)
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
	if((bind(tsock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		printf("Error SockBind\n");
		closesocket(tsock);
		return 0;
	}
	//�ڑ��ҋ@��Ԃɂ���
	if((listen(tsock,SOMAXCONN)) == SOCKET_ERROR)
	{
		printf("Error SockListen\n");
		closesocket(tsock);
		return 0;
	}
	//�ڑ����Ă����A�h���X���
	SOCKADDR_IN ta;
	//�A�h���X�\���̂̃T�C�Y
	int addrin_size = sizeof(SOCKADDR_IN);
	//�ڑ��҂�
	printf("Wait Accept\n");
	SOCKET ts = accept(tsock,(struct sockaddr*)&ta,&addrin_size);
	printf("Client IP %s\n",inet_ntoa(ta.sin_addr));
	//�\�P�b�g�̃u���b�L���O����������
	u_long val = 1;
	ioctlsocket(ts, FIONBIO, &val);
	//��M
	while(TRUE)
	{
		char msg[1024];
		memset(msg,0,sizeof(msg));

		int act;
		printf("Press ChatAction(0:Receive,1:Send,2:End): ");
		scanf("%d",&act);
		if(act == 2)
		{
			break;
		}
		else if(act == 0)
		{
			//���b�Z�[�W������Ԏ�M�p��
			while(TRUE)
			{
				int s = recv(ts,msg,sizeof(msg),0);
				if(s > 0)
				{
					printf("%d Bytes Receive\n",s);
					printf("Receive Message : %s\n",msg);
				}
				else
				{
					printf("No Message\n");
					break;
				}
			}
		}
		else
		{
			//���M
			printf("Press Send Message : ");
			scanf("%s",msg);
			int s = send(ts,msg,strlen(msg) + 1,0);
			printf("%d Bytes Send\n",s);
		}
	}
	//��n��
	closesocket(ts);
	closesocket(tsock);
	WSACleanup();
	return 0;
}