#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>

//�\�P�b�g�ʐM�p
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

//�X���b�h�p
#include	<process.h>
#include	<windows.h>

// ********************************************************************************
/// <summary>
/// �\�P�b�g�G���[��
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
/// �\�P�b�g�ʐM�̏��
/// </summary>
// ********************************************************************************
enum WSASTATE
{
    WSASTATE_NONE,
    WSASTATE_INIT,
    WSASTATE_CLEANUP,
};

extern WSASTATE g_WsaState;