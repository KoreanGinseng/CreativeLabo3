#pragma once
#include "SocketDefine.h"

// ********************************************************************************
/// <summary>
/// ソケット通信用インターフェース
/// </summary>
// ********************************************************************************
template< class T >
class CMySocket
{
protected:

    SOCKET      m_Socket;                               //! ソケット
    SOCKETERROR m_Error{ SOCKETERROR::ERROR_NONE };     //! ソケットエラー
    int         m_PortNo;                               //! ポート番号
    bool        m_bStart;                               //! 開始フラグ

public:

    CMySocket(void) = default;
    
    // ********************************************************************************
    /// <summary>
    /// デストラクタ
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual ~CMySocket(void)
    {
        if (g_WsaState != WSASTATE_CLEANUP)
        {
            g_WsaState = WSASTATE_CLEANUP;
            WSACleanup();
        }
    }

    // ********************************************************************************
    /// <summary>
    /// WSA初期化
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void Init(void)
    {
        //WSAStartUp
        if (g_WsaState != WSASTATE_INIT)
        {
            g_WsaState = WSASTATE_INIT;
            WSAData wsaData;
            int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (ret != 0)
            {
                m_Error = SOCKETERROR::ERROR_INIT;
            }
        }
    }

    // ********************************************************************************
    /// <summary>
    /// 開始
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void Start(void)
    {
        Init();
        m_bStart = true;
    }

    // ********************************************************************************
    /// <summary>
    /// 開始フラグの取得
    /// </summary>
    /// <returns>true : 動作中, false : 動作前</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    const bool IsStart(void) const noexcept
    {
        return m_bStart;
    }

    // ********************************************************************************
    /// <summary>
    /// 送信
    /// </summary>
    /// <param name="data">送信データ</param>
    /// <returns>送信データサイズ</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual int Send(const T& data)
    {
        return send(m_Socket, (char*)&data, sizeof(T), 0);
    }

    // ********************************************************************************
    /// <summary>
    /// 受信データを好きにする関数
    /// </summary>
    /// <param name="data">受信データ</param>
    /// <param name="size">受信データサイズ</param>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void Recieve(const T& data, int size) = 0;

    // ********************************************************************************
    /// <summary>
    /// operator()
    /// </summary>
    /// <returns>true : 正常, false : 異常</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    const bool operator()(void) const noexcept
    {
        return (m_Error == SOCKETERROR::ERROR_NONE);
    }

    // ********************************************************************************
    /// <summary>
    /// エラーの取得
    /// </summary>
    /// <returns>SOCKETERROR</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    const SOCKETERROR GetError(void) const noexcept
    {
        return m_Error;
    }
};