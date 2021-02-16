#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>

//ソケット通信用
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

//スレッド用
#include	<process.h>
#include	<windows.h>

// ********************************************************************************
/// <summary>
/// ソケットエラー列挙
/// </summary>
// ********************************************************************************
enum SOCKETERROR
{
	ERROR_NONE,
	ERROR_INIT,
	ERROR_CREATE,
	ERROR_BIND,
	ERROR_LISTEN,
	ERROR_CONNECT,
};

// ********************************************************************************
/// <summary>
/// ソケット通信の状態
/// </summary>
// ********************************************************************************
enum WSASTATE
{
    WSASTATE_NONE,
    WSASTATE_INIT,
    WSASTATE_CLEANUP,
};

extern WSASTATE g_WsaState;