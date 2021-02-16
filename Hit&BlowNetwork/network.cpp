//�C���N���[�h
//���̃v���O�����Ŏg�p���Ă���֐���
//��`����Ă���t�@�C�����v���O�����Ɋ܂߂܂�
#define		_CRT_SECURE_NO_WARNINGS
#include	<stdio.h>

//�\�P�b�g�ʐM�p
#define		_WINSOCK_DEPRECATED_NO_WARNINGS
#include	<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

/*
 * ���͂���������3�̐����ɕ�������
 */
bool Parse(int n, int& i1, int& i2, int& i3)
{
	if (n >= 1000 || n < 100)
	{
		printf("�R���̐�������͂��Ă�������\n");
		return false;
	}
	i1 = n % 10;	n /= 10;
	i2 = n % 10;	n /= 10;
	if (i1 == i2)
	{
		printf("3���ɓ��������͎g���܂���\n");
		return false;
	}
	i3 = n % 10;
	if (i1 == i3 || i2 == i3)
	{
		printf("3���ɓ��������͎g���܂���\n");
		return false;
	}
	return true;
}

/*
 * ����̐����������̐����ƈ�v���邩���肷��
 */
bool Answer(int i1, int i2, int i3, int a1, int a2, int a3) {
	int hit = 0;
	int blow = 0;
	//�����̐����𔻒肷��
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
 * ���C���֐�
 * �v���O�����͂�������X�^�[�g���܂�
 */
int main(void)
{
	//�����̓��͂������Ȃ�
	printf("���Ȃ��̓����̐�������͂��Ă�������:");
	int ans = 0;
	scanf("%d", &ans);
	printf("���Ȃ��̓�����%d�ł�\n", ans);
	int self1, self2, self3;
	if (!Parse(ans, self1, self2, self3)) { return 0; }

	//WSAStartUp
	WSAData wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	//�G���[�`�F�b�N
	if (ret != 0)
	{
		printf("WinSock�̏������Ɏ��s\n");
		return 0;
	}

	//�\�P�b�g�̍쐬
	SOCKET tsock = socket(AF_INET, SOCK_STREAM, 0);
	if (tsock == INVALID_SOCKET)
	{
		perror("�\�P�b�g�������s\n");
		return 0;
	}

	//�z�X�g���N���C�A���g���q�˂�
	int term;
	printf("���̐l�̐ڑ���҂� : 0\n���̐l�ɐڑ����� : 1\n");
	scanf("%d", &term);

	//�A�h���X�\����
	struct sockaddr_in tmp_addr;
	memset(&tmp_addr, 0, sizeof(struct sockaddr_in));
	//�l�b�g���[�N�̃f�[�^��ݒ�
	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_port = htons(18900);

	//�z�X�g�ƃN���C�A���g�ŏ�����ς���
	if (term == 0)
	{
		tmp_addr.sin_addr.s_addr = ADDR_ANY;

		//�A�h���X�ƃ\�P�b�g���o�C���h
		if ((bind(tsock, (struct sockaddr*) & tmp_addr, sizeof(struct sockaddr_in))) == SOCKET_ERROR)
		{
			perror("bind���s\n");
			closesocket(tsock);
			return 0;
		}

		//�ڑ��ҋ@��Ԃɂ���
		if ((listen(tsock, SOMAXCONN)) == SOCKET_ERROR)
		{
			perror("listen���s\n");
			closesocket(tsock);
			return 0;
		}

		//�ڑ����Ă����A�h���X���
		SOCKADDR_IN ta;
		int addrin_size = sizeof(SOCKADDR_IN);
		//�ڑ��҂�
		printf("�ڑ��ҋ@��...\n");
		SOCKET ts = accept(tsock, (struct sockaddr*) & ta, &addrin_size);

		//�ڑ��p�\�P�b�g����āA�ڑ����Ă����\�P�b�g������͎g�p����
		closesocket(tsock);
		tsock = ts;
	}
	else if (term == 1)
	{
		//�ڑ�IP�����
		char ip[256];
		printf("�ڑ����IP�A�h���X�����Ă������� IP : ");
		scanf("%s", ip);
		tmp_addr.sin_addr.s_addr = inet_addr(ip);

		//�ڑ�
		if (connect(tsock, (struct sockaddr*) & tmp_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		{
			perror("�ڑ����s\n");
			closesocket(tsock);
			return 0;
		}
	}

	//�ŏ��Ɍ��߂�3���̐����𑗐M����
	send(tsock, (char*)&ans, sizeof(int), 0);

	//���肩��̐�������M����
	int eno;
	recv(tsock, (char*)&eno, sizeof(int), 0);
	//����̓����̐���
	int ans1 = 0, ans2 = 0, ans3 = 0;
	ans1 = eno % 10; eno /= 10;
	ans2 = eno % 10; eno /= 10;
	ans3 = eno % 10; eno /= 10;

	//�ǂ���̎��
	int now = 0;
	//���͉�
	int cnt = 0;

	//��������܂ŌJ��Ԃ�
	while (true)
	{
		int no = 0;
		int in1, in2, in3;

		if (term == now % 2)
		{
			//�����̓��͂������Ȃ�
			printf("\n%d���:3���̐�������͂��Ă�������:", ++cnt);
			scanf("%d", &no);
			printf("���Ȃ��̓�����%d�ł�\n", no);
			if (!Parse(no, in1, in2, in3)) { continue; }
			//���߂�3���̐����𑗐M����
			send(tsock, (char*)&no, sizeof(int), 0);

			//�����̐����𔻒肷��
			if (Answer(in1, in2, in3, ans1, ans2, ans3))
			{
				printf("����!!\n");
				printf("���Ȃ��̏���\n");
				break;
			}
		}
		else
		{
			//���肩��̐�������M����
			printf("\n���肪�������l���Ă��܂�...\n");
			recv(tsock, (char*)&no, sizeof(int), 0);
			printf("����̓�����%d�ł�\n", no);
			in1 = no % 10; no /= 10;
			in2 = no % 10; no /= 10;
			in3 = no % 10; no /= 10;

			//�����̐����𔻒肷��
			if (Answer(in1, in2, in3, self1, self2, self3))
			{
				printf("����!!\n");
				printf("���Ȃ��̕���\n");
				break;
			}
		}
		now++;
	}

	//��n��
	closesocket(tsock);
	WSACleanup();

	return 0;
}