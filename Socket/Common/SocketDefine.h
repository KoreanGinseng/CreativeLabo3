#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>

//ソケット通信用
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

enum SOCKETERROR
{
	ERROR_NONE,
	ERROR_INIT,
	ERROR_CREATE,
	ERROR_BIND,
	ERROR_LISTEN,
	ERROR_CONNECT,
};