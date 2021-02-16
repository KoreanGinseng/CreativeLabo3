//インクルード
//このプログラムで使用している関数が
//定義されているファイルをプログラムに含めます
#define		_CRT_SECURE_NO_WARNINGS
#include	<stdio.h>

//ソケット通信用
#define		_WINSOCK_DEPRECATED_NO_WARNINGS
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

/*
 * 入力した数字を3つの数字に分解する
 */
bool Parse(int n, int& i1, int& i2, int& i3)
{
	if (n >= 1000 || n < 100)
	{
		printf("３桁の整数を入力してください\n");
		return false;
	}
	i1 = n % 10;	n /= 10;
	i2 = n % 10;	n /= 10;
	if (i1 == i2)
	{
		printf("3桁に同じ数字は使えません\n");
		return false;
	}
	i3 = n % 10;
	if (i1 == i3 || i2 == i3)
	{
		printf("3桁に同じ数字は使えません\n");
		return false;
	}
	return true;
}

/*
 * 相手の数字が自分の正解と一致するか判定する
 */
bool Answer(int i1, int i2, int i3, int a1, int a2, int a3) {
	int hit = 0;
	int blow = 0;
	//数字の正解を判定する
	if (i1 == a1)
	{
		hit++;
	}
	if (i1 == a2 || i1 == a3)
	{
		blow++;
	}

	if (i2 == a2)
	{
		hit++;
	}
	if (i2 == a1 || i2 == a3)
	{
		blow++;
	}

	if (i3 == a3)
	{
		hit++;
	}
	if (i3 == a1 || i3 == a2)
	{
		blow++;
	}
	printf("Hit : %d, Blow : %d\n", hit, blow);
	return hit >= 3;
}

/**
 * メイン関数
 * プログラムはここからスタートします
 */
int main(void)
{
	//数字の入力をおこなう
	printf("あなたの答えの数字を入力してください:");
	int ans = 0;
	scanf("%d", &ans);
	printf("あなたの答えは%dです\n", ans);
	int self1, self2, self3;
	if (!Parse(ans, self1, self2, self3)) { return 0; }

	//WSAStartUp
	WSAData wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//エラーチェック
	if (ret != 0)
	{
		printf("WinSockの初期化に失敗\n");
		return 0;
	}

	//ソケットの作成
	SOCKET tsock = socket(AF_INET, SOCK_STREAM, 0);
	if (tsock == INVALID_SOCKET)
	{
		perror("ソケット生成失敗\n");
		return 0;
	}

	//ホストかクライアントか尋ねる
	int term;
	printf("他の人の接続を待つ : 0\n他の人に接続する : 1\n");
	scanf("%d", &term);

	//アドレス構造体
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
	//ネットワークのデータを設定
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);

	//ホストとクライアントで処理を変える
	if (term == 0)
	{
		tmp_addr.sin_addr.s_addr = ADDR_ANY;

		//アドレスとソケットをバインド
		if ((bind(tsock, (struct sockaddr*) & tmp_addr, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
		{
			perror("bind失敗\n");
			closesocket(tsock);
			return 0;
		}

		//接続待機状態にする
		if ((listen(tsock, SOMAXCONN)) == SOCKET_ERROR)
		{
			perror("listen失敗\n");
			closesocket(tsock);
			return 0;
		}

		//接続してきたアドレス情報
		SOCKADDR_IN ta;
		int addrin_size = sizeof(SOCKADDR_IN);
		//接続待ち
		printf("接続待機中...\n");
		SOCKET ts = accept(tsock, (struct sockaddr*) & ta, &addrin_size);

		//接続用ソケットを閉じて、接続してきたソケットを今後は使用する
		closesocket(tsock);
		tsock = ts;
	}
	else if (term == 1)
	{
		//接続IPを入力
		char ip[256];
		printf("接続先のIPアドレスを入れてください IP : ");
		scanf("%s", ip);
		tmp_addr.sin_addr.s_addr = inet_addr(ip);

		//接続
		if (connect(tsock, (struct sockaddr*) & tmp_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			perror("接続失敗\n");
			closesocket(tsock);
			return 0;
		}
	}

	//最初に決めた3桁の数字を送信する
	send(tsock, (char*)&ans, sizeof(int), 0);

	//相手からの数字を受信する
	int eno;
	recv(tsock, (char*)&eno, sizeof(int), 0);
	//相手の答えの数字
	int ans1 = 0, ans2 = 0, ans3 = 0;
	ans1 = eno % 10; eno /= 10;
	ans2 = eno % 10; eno /= 10;
	ans3 = eno % 10; eno /= 10;

	//どちらの手番
	int now = 0;
	//入力回数
	int cnt = 0;

	//正解するまで繰り返し
	while (true)
	{
		int no = 0;
		int in1, in2, in3;

		if (term == now % 2)
		{
			//数字の入力をおこなう
			printf("\n%d回目:3桁の数字を入力してください:", ++cnt);
			scanf("%d", &no);
			printf("あなたの答えは%dです\n", no);
			if (!Parse(no, in1, in2, in3)) { continue; }
			//決めた3桁の数字を送信する
			send(tsock, (char*)&no, sizeof(int), 0);

			//数字の正解を判定する
			if (Answer(in1, in2, in3, ans1, ans2, ans3))
			{
				printf("正解!!\n");
				printf("あなたの勝ち\n");
				break;
			}
		}
		else
		{
			//相手からの数字を受信する
			printf("\n相手が数字を考えています...\n");
			recv(tsock, (char*)&no, sizeof(int), 0);
			printf("相手の答えは%dです\n", no);
			in1 = no % 10; no /= 10;
			in2 = no % 10; no /= 10;
			in3 = no % 10; no /= 10;

			//数字の正解を判定する
			if (Answer(in1, in2, in3, self1, self2, self3))
			{
				printf("正解!!\n");
				printf("あなたの負け\n");
				break;
			}
		}
		now++;
	}

	//後始末
	closesocket(tsock);
	WSACleanup();

	return 0;
}