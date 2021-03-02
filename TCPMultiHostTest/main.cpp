#include	<stdio.h>

//ソケット通信用
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//スレッド用
#include	<process.h>
#include	<windows.h>

//クライアント構造体
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
	//ソケットの作成
	AcceptSock = socket(AF_INET, SOCK_STREAM, 0);
	if(AcceptSock == INVALID_SOCKET)
	{
		printf("Error SockCreate\n");
		return 0;
	}
	//アドレス構造体
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr,0,sizeof(struct sockaddr_in));
	//ネットワークのデータを設定
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);
	tmp_addr.sin_addr.s_addr = ADDR_ANY;
	//アドレスとソケットをバインド
	if((bind(AcceptSock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		printf("Error SockBind\n");
		closesocket(AcceptSock);
		return 0;
	}
	//接続待機状態にする
	if((listen(AcceptSock,SOMAXCONN)) == SOCKET_ERROR)
	{
		printf("Error SockListen\n");
		closesocket(AcceptSock);
		return 0;
	}

	while(TRUE)
	{
		//接続してきたアドレス情報
		SOCKADDR_IN ta;
		//アドレス構造体のサイズ
		int addrin_size = sizeof(SOCKADDR_IN);
		//接続待ち
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
			//受信スレッドの開始
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

	//クライアントの情報を初期化
	memset(Client,0,sizeof(Client));
	//接続待機スレッドの開始
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,AcceptThread,"Thread1",0,NULL);

	//受信
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
	//後始末
	closesocket(AcceptSock);
	for(int i = 0;i < MAXCLIENT;i++)
	{
		if(!Client[i].bConnect)
		{
			continue;
		}
		closesocket(Client[i].Socket);
	}
	//スレッドの停止待機
	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread1);
	WSACleanup();
	return;
}