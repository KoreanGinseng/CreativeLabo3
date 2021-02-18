#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

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
/// 送信タイプ
/// </summary>
// ********************************************************************************
enum SENDTYPE
{
    SENDTYPE_BROADCAST,   //! 自分を含む全員に送信
    SENDTYPE_OTHERCAST,   //! 自分以外の全員に送信
    SENDTYPE_UNIQUECAST,  //! 個人に送信
    SENDTYPE_MULTICAST,   //! 複数に送信
    SENDTYPE_OWNERCAST,   //! オーナーに送信
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