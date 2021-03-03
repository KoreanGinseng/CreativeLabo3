#pragma once

#include	"Socket.h"
#include	"../FrameTimer.h"

namespace Sample {

	class RUDPMessage {
	public:
		enum class ResultType {
			None,												//!<返信不要
			Request,											//!<返信要求
			Succeeded,											//!<成功
		};

#pragma pack(push, 1)
		struct Header {
			uint32_t							Type;			//!<メッセージタイプ
			uint32_t							Size;			//!<メッセージサイズ
			uint32_t							No;				//!<メッセージ番号
			ResultType							Result;			//!<メッセージ結果
		};

		struct Result {
			double								Time;			//!<時間
			double								ReTime;			//!<再送時間
			uint32_t							ReCount;		//!<再送回数
			UdpSocketPtr						Socket;			//!<ソケット
			SocketAddr							Addr;			//!<アドレス
			void*								pBuffer;		//!<メッセージバッファ
			uint32_t							BufSize;		//!<メッセージバッファサイズ

			Result() 
			: Time(0)
			, ReTime(0)
			, ReCount(0)
			, Socket()
			, Addr()
			, pBuffer(nullptr) 
			, BufSize(0) {
			}

			~Result() {
				SAFE_DELETE_ARRAY(pBuffer);
			}
		};
		using ResultPtr = std::shared_ptr<Result>;
#pragma pack(pop)

		struct MessageData {
			/** 受信データ */
			char*									pData;
			/** 受信ヘッダー */
			Header									header;

			~MessageData() {
				SAFE_DELETE_ARRAY(pData);
			}
		};
		using MessageDataPtr = std::shared_ptr<MessageData>;

		struct RUDPAddr {
			/** 受信アドレス */
			SocketAddr								addr;
			/** UDPメッセージ番号 */
			uint32_t								MessageNo;
			/** RUDPメッセージ番号 */
			uint32_t								RequestMessageNo;
			/** RUDPメッセージ受信番号 */
			uint32_t								ReceiveNo;
			/** 受信データ */
			std::list< MessageDataPtr >				MessageDatas;
			/** 最終通信時間 */
			double									lastTime;

			RUDPAddr(SocketAddr a)
			: addr(a) 
			, MessageNo(0)
			, RequestMessageNo(0)
			, ReceiveNo(0)
			, MessageDatas() 
			, lastTime(FrameTimer::Instance().NowTime()) {
			}

			/**
			 * @brief		該当番号のメッセージ存在確認
			 */
			bool IsMessage(uint32_t n)
			{
				const auto& it = std::find_if(MessageDatas.begin(), MessageDatas.end(),
					[n](const MessageDataPtr& data) {
						return data->header.No == n;
					});
				return it != MessageDatas.end();
			}
		};
		using RUDPAddrPtr = std::shared_ptr<RUDPAddr>;

	private:
		/** 接続アドレスごとの送信管理 */
		std::list< RUDPAddrPtr >				ConnectAddr_;
		/** 再送試行回数 */
		uint32_t								RetryMax_;
		/** アドレス削除までの時間 */
		double									DeleteTime;
		/** 再送確認が必要なデータリスト */
		std::list< ResultPtr >					ResultList_;
	public:
		RUDPMessage()
		: ConnectAddr_()
		, RetryMax_(50) 
		, DeleteTime(300)
		, ResultList_() {
		}

		~RUDPMessage() {
		}

		bool Update() {
			double nt = FrameTimer::Instance().NowTime();
			for(auto& it : ResultList_)
			{
				double et = nt - it->Time;
				if (et > it->ReTime)
				{
					it->Socket->Send(it->Addr, it->pBuffer, it->BufSize);
					it->Time = nt;
					it->ReCount++;
				}
			}
			ResultList_.erase(std::remove_if(ResultList_.begin(), ResultList_.end(),
				[this](const ResultPtr& act) { return act->ReCount >= RetryMax_; }), ResultList_.end());
			//長期間無通信の削除
			ConnectAddr_.erase(std::remove_if(ConnectAddr_.begin(), ConnectAddr_.end(),
				[this, nt](const RUDPAddrPtr& it) {
					return (nt - it->lastTime) >= DeleteTime;
				}), ConnectAddr_.end());
			return TRUE;
		}

		/*
		 * @brief			管理アドレスの取得
		 * @param[in]		addr			アドレス
		 */
		RUDPAddrPtr GetAddr(const SocketAddr& addr)
		{
			//アドレス情報検索
			const auto& it = std::find_if(ConnectAddr_.begin(), ConnectAddr_.end(),
				[addr](const RUDPAddrPtr& data) {
					return data->addr.sin_addr.S_un.S_addr == addr.sin_addr.S_un.S_addr && data->addr.sin_port == addr.sin_port;
				});
			//存在しない場合新規作成
			if (it == ConnectAddr_.end())
			{
				RUDPAddrPtr saddr = std::make_shared<RUDPAddr>(addr);
				ConnectAddr_.push_back(saddr);
				return saddr;
			}
			return *it;
		}

		/**
		 * @brief			データの送信
		 * @param[in]		sock			送信ソケット
		 * @param[in]		addr			送信アドレス
		 * @param[in]		pData			送信バッファ
		 * @param[in]		Size			送信バッファサイズ
		 * @param[in]		type			送信メッセージ識別子
		 * @param[in]		ReTime			再送までの時間
		 */
		bool Send(UdpSocketPtr& sock, const SocketAddr& addr, const void* pData, uint32_t Size, uint32_t type, double ReTime = 0) {
			//アドレス情報検索
			RUDPAddrPtr saddr = GetAddr(addr);
			saddr->lastTime = FrameTimer::Instance().NowTime();
			//受信データ作成
			uint32_t msize = Size + sizeof(Header);
			uint8_t* pBuf = new uint8_t[msize];
			Header* pHeader = (Header*)pBuf;
			pHeader->No = ((ReTime > 0) ? saddr->RequestMessageNo++ : saddr->MessageNo++);
			pHeader->Result = ((ReTime > 0) ? ResultType::Request : ResultType::None);
			pHeader->Size = Size;
			pHeader->Type = type;
			memcpy(&pBuf[sizeof(Header)], pData, Size);
			sock->Send(addr, pBuf, msize);
			if (ReTime > 0)
			{
				ResultPtr pAdd = std::make_shared<Result>();
				pAdd->Socket = sock;
				pAdd->Addr = addr;
				pAdd->Time = FrameTimer::Instance().NowTime();
				pAdd->ReTime = ReTime;
				pAdd->ReCount = 0;
				pAdd->pBuffer = pBuf;
				pAdd->BufSize = msize;
				ResultList_.push_back(pAdd);
				return true;
			}
			SAFE_DELETE_ARRAY(pBuf);
			return true;
		}

		/**
		 * @brief			データの送信
		 * @param[in/out]	pData			受信データ
		 * @param[in]		Size			受信データサイズ
		 * @param[in/out]	oheader			受信メッセージヘッダー
		 * @param[in/out]	raddr			受信アドレス
		 * @param[in]		os				通信ソケット
		 */
		bool Receive(void* pData, uint32_t Size, Header& oheader, SocketAddr& raddr, UdpSocketPtr& os) {
			//処理対象データが残っているならそこを処理
			const auto& rit = std::find_if(ConnectAddr_.begin(), ConnectAddr_.end(),
				[](const RUDPAddrPtr& data) {
					return data->IsMessage(data->ReceiveNo);
				});
			if (rit != ConnectAddr_.end())
			{
				const auto& mit = std::find_if((*rit)->MessageDatas.begin(), (*rit)->MessageDatas.end(),
					[rit](const MessageDataPtr& data) {
						return data->header.No == (*rit)->ReceiveNo;
					});
				raddr = (*rit)->addr;
				oheader = (*mit)->header;
				memcpy(pData, (*mit)->pData, oheader.Size);
				(*rit)->MessageDatas.erase(mit);
				(*rit)->ReceiveNo++;
				return true;
			}
			//受信データはない
			if (!os->IsReceive())
			{
				return false;
			}
			uint32_t rsize = os->Receive(pData, Size, raddr);
			char* buf = (char*)pData;
			memcpy(&oheader, buf, sizeof(Header));
			memcpy(buf, &buf[sizeof(Header)], oheader.Size);
			//返信
			if (oheader.Result == ResultType::Request)
			{
				//受信確認返信
				Header rehd = oheader;
				rehd.Size = 0;
				rehd.Result = ResultType::Succeeded;
				os->Send(raddr, &rehd, sizeof(Header));
				//アドレス情報検索
				RUDPAddrPtr saddr = GetAddr(raddr);
				saddr->lastTime = FrameTimer::Instance().NowTime();
				//返信要求のメッセージをストック
				if (saddr->ReceiveNo < oheader.No)
				{
					if (!saddr->IsMessage(oheader.No))
					{
						MessageDataPtr msg = std::make_shared<MessageData>();
						msg->pData = new char[oheader.Size];
						memcpy(msg->pData, buf, oheader.Size);
						msg->header = oheader;
						saddr->MessageDatas.push_back(msg);
					}
					return Receive(pData, Size, oheader, raddr, os);
				}
				//処理済みのデータのため破棄
				else if (saddr->ReceiveNo > oheader.No)
				{
					return Receive(pData, Size, oheader, raddr, os);
				}
				//処理対象データなので位置を進める
				else
				{
					saddr->ReceiveNo++;
				}
			}
			//返信確認
			else if (oheader.Result == ResultType::Succeeded)
			{
				ResultList_.erase(std::remove_if(ResultList_.begin(), ResultList_.end(),
					[&](const ResultPtr& act) {
						return oheader.No == ((Header*)act->pBuffer)->No &&
							act->Addr.sin_addr.S_un.S_addr == raddr.sin_addr.S_un.S_addr && act->Addr.sin_port == raddr.sin_port;;
					}), ResultList_.end());
				return Receive(pData, Size, oheader, raddr, os);
			}
			return true;
		}
	};
}