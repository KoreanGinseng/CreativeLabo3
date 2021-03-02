#define		_WINSOCK_DEPRECATED_NO_WARNINGS
#define		_CRT_SECURE_NO_WARNINGS

#include	<stdio.h>

//ソケット通信用
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <string>

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

	//接続IPを入力
	char ip[256] = "127.0.0.1";
	printf("Press Connection IP : ");
	//scanf("%s",ip);
	//アドレス構造体
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr,0,sizeof(struct sockaddr_in));
	//ネットワークのデータを設定
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);
	tmp_addr.sin_addr.s_addr = inet_addr(ip);
	if(connect(tsock,(struct sockaddr*)&tmp_addr,sizeof(struct sockaddr_in)) == SOCKET_ERROR)
	{
		closesocket(tsock);
		printf("Error Connect\n");
		return 0;
	}
	//ソケットのブロッキングを解除する
	u_long val = 1;
	ioctlsocket(tsock, FIONBIO, &val);

	//送信
	while(TRUE)
	{
		char msg[10];
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
			int         msglength = 0;
			std::string overmsg;
			//メッセージがある間受信継続
			while(TRUE)
			{
				int s = recv(tsock,msg,sizeof(msg),0);
				if(s > 0)
				{
					for (int i = 0; i < s; i++)
					{
						overmsg += msg[i];
					}
					msglength += s;
					if (msg[s - 1] != '\0')
					{
						continue;
					}
					for (int i = 0; i < msglength; i = (overmsg.find('\0', i) + 1))
					{
						std::string str = &(overmsg[i]);
						printf("%d Bytes Receive\n", str.length() + 1);
						printf("Receive Message : %s\n", str.c_str());
					}
					overmsg   = "";
					msglength =  0;
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
			int s = send(tsock,msg,strlen(msg) + 1,0);
			printf("%d Bytes Send\n",s);
		}
	}
	//後始末
	closesocket(tsock);
	WSACleanup();
	return 0;
}