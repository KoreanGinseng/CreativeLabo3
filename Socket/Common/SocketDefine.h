#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

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
/// ���M�^�C�v
/// </summary>
// ********************************************************************************
enum SENDTYPE
{
    SENDTYPE_BROADCAST,   //! �������܂ޑS���ɑ��M
    SENDTYPE_OTHERCAST,   //! �����ȊO�̑S���ɑ��M
    SENDTYPE_UNIQUECAST,  //! �l�ɑ��M
    SENDTYPE_MULTICAST,   //! �����ɑ��M
    SENDTYPE_OWNERCAST,   //! �I�[�i�[�ɑ��M
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

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
struct DataHeader
{
    int Type;
    int Size;
};

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
enum DATATYPE
{
    DATATYPE_SETID   = -100,
};

extern WSASTATE g_WsaState;