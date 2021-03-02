#include	<stdio.h>

//ソケット通信用
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

//スレッド用
#include	<process.h>
#include	<windows.h>

//ソケットをスレッドでも使うためグローバル
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

	//ソケットの作成
	SOCKET tsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tsock == INVALID_SOCKET)
	{
		printf("Error SockCreate\n");
		return;
	}
	//アドレス構造体
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr,0,sizeof(struct sockaddr_in));
	//ネットワークのデータを設定
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);
	tmp_addr.sin_addr.s_addr = ADDR_ANY;
	//アドレスとソケットをバインド
	if((bind(tsock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		printf("Error SockBind\n");
		closesocket(tsock);
		return;
	}
	//接続待機状態にする
	if((listen(tsock,SOMAXCONN)) == SOCKET_ERROR)
	{
		printf("Error SockListen\n");
		closesocket(tsock);
		return;
	}
	//接続してきたアドレス情報
	SOCKADDR_IN ta;
	//アドレス構造体のサイズ
	int addrin_size = sizeof(SOCKADDR_IN);
	//接続待ち
	printf("Wait Accept\n");
	ts = accept(tsock,(struct sockaddr*)&ta,&addrin_size);
	printf("Client IP %s\n",inet_ntoa(ta.sin_addr));
	//受信スレッドの開始
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,Thread,"Thread1",0,NULL);
	//ソケットのブロッキングを解除する
	u_long val = 1;
	ioctlsocket(ts, FIONBIO, &val);
	//受信
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
	//スレッドの停止待機
	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread1);
	//後始末
	closesocket(ts);
	closesocket(tsock);
	WSACleanup();
	return;
}