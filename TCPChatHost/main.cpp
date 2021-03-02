#define		_WINSOCK_DEPRECATED_NO_WARNINGS
#define		_CRT_SECURE_NO_WARNINGS

#include	<stdio.h>

//ソケット通信用
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

int main(void)
{
	//WSAStartUp
	WSAData wsaData;
	int ret = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(ret != 0)
	{
		printf("WinSockの初期化に失敗\n");
	}

	//ソケットの作成
	SOCKET tsock = socket(AF_INET, SOCK_STREAM, 0);
	if(tsock == INVALID_SOCKET)
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
	if((bind(tsock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in))) == SOCKET_ERROR)
	{
		printf("Error SockBind\n");
		closesocket(tsock);
		return 0;
	}
	//接続待機状態にする
	if((listen(tsock,SOMAXCONN)) == SOCKET_ERROR)
	{
		printf("Error SockListen\n");
		closesocket(tsock);
		return 0;
	}
	//接続してきたアドレス情報
	SOCKADDR_IN ta;
	//アドレス構造体のサイズ
	int addrin_size = sizeof(SOCKADDR_IN);
	//接続待ち
	printf("Wait Accept\n");
	SOCKET ts = accept(tsock,(struct sockaddr*)&ta,&addrin_size);
	printf("Client IP %s\n",inet_ntoa(ta.sin_addr));
	//ソケットのブロッキングを解除する
	u_long val = 1;
	ioctlsocket(ts, FIONBIO, &val);
	//受信
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
			//メッセージがある間受信継続
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
			//送信
			printf("Press Send Message : ");
			scanf("%s",msg);
			int s = send(ts,msg,strlen(msg) + 1,0);
			printf("%d Bytes Send\n",s);
		}
	}
	//後始末
	closesocket(ts);
	closesocket(tsock);
	WSACleanup();
	return 0;
}