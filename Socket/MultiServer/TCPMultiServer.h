#pragma once
#include "../Common/MySocket.h"
#include <vector>

// ********************************************************************************
/// <summary>
/// クライアント
/// </summary>
// ********************************************************************************
struct ClientData
{
	bool      bConnect{ false };
    CMySocket Socket;
};

// ********************************************************************************
/// <summary>
/// 複数人接続サーバー
/// </summary>
// ********************************************************************************
class CTCPMultiServer
{
protected:

    CMySocket				          m_Accept;
    std::vector<ClientData>           m_Client{ 10, ClientData() };
    std::vector<std::pair<int, bool>> m_IdList{ 10, std::pair<int, bool>(0, false) };
    HANDLE                            m_hAcceptThread;
    int                               m_OwnerId{ -1 };

public:

    // ********************************************************************************
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="bStart">通信開始フラグ</param>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    explicit CTCPMultiServer(unsigned int multiCount = 10, int portNo = 18900, bool bStart = false);

    // ********************************************************************************
    /// <summary>
    /// デストラクタ
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual ~CTCPMultiServer(void);

    // ********************************************************************************
    /// <summary>
    /// 開始
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    void Start(void);

    // ********************************************************************************
    /// <summary>
    /// 送信
    /// </summary>
    /// <param name="pData">送信データ</param>
    /// <param name="datalen"></param>
    /// <param name="sendType"></param>
    /// <param name="ids"></param>
    /// <param name="idlen"></param>
    /// <returns></returns>
    /// <created>いのうえ,2021/02/18</created>
    /// <changed>いのうえ,2021/02/18</changed>
    // ********************************************************************************
    int Send(const void* pData, int datalen, const SENDTYPE& sendType = SENDTYPE_BROADCAST, int* ids = nullptr, int idlen = 0);

    // ********************************************************************************
    /// <summary>
    /// 受信用スレッド
    /// </summary>
    /// <param name="pData">this</param>
    /// <returns>0 : 正常終了, それ以外 : エラー</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    static unsigned int WINAPI RecieveThread(void* pData);

    // ********************************************************************************
    /// <summary>
    /// 接続用スレッド
    /// </summary>
    /// <param name="pData">this</param>
    /// <returns>0 : 正常終了, それ以外 : エラー</returns>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    static unsigned int WINAPI AcceptThread(void* pData);

    // ********************************************************************************
    /// <summary>
    /// エラーの取得
    /// </summary>
    /// <returns>エラーの取得</returns>
    /// <created>いのうえ,2021/02/17</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    inline const SOCKETERROR GetError(void) const
    {
        return m_Accept.GetError();
    }

    // ********************************************************************************
    /// <summary>
    /// 接続された瞬間に1度だけ実行される関数
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void Connect(void) {}

    // ********************************************************************************
    /// <summary>
    /// 接続が切れた瞬間に1度だけ実行される関数
    /// </summary>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/16</changed>
    // ********************************************************************************
    virtual void DisConnect(void) {}

    // ********************************************************************************
    /// <summary>
    /// 受信データを好きにする関数
    /// </summary>
    /// <param name="header">データヘッダ</param>
    /// <param name="data">受信データ</param>
    /// <param name="datalen">受信データサイズ</param>
    /// <created>いのうえ,2021/02/16</created>
    /// <changed>いのうえ,2021/02/17</changed>
    // ********************************************************************************
    virtual void Recieve(const DataHeader& header, const void* data, int datalen) {}

private:

    int SendBroadCast(const void* pData, int datalen);
    int SendOtherCast(const void* pData, int datalen, int sendid);
    int SendUniqueCast(const void* pData, int datalen, int id);
    int SendMultiCast(const void* pData, int datalen, int* ids, int idlen);
    int SendOwnerCast(const void* pData, int datalen);
};
