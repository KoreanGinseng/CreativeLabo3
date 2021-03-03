#pragma once

#include	"Socket.h"
#include	"../FrameTimer.h"

namespace Sample {

	class RUDPMessage {
	public:
		enum class ResultType {
			None,												//!<�ԐM�s�v
			Request,											//!<�ԐM�v��
			Succeeded,											//!<����
		};

#pragma pack(push, 1)
		struct Header {
			uint32_t							Type;			//!<���b�Z�[�W�^�C�v
			uint32_t							Size;			//!<���b�Z�[�W�T�C�Y
			uint32_t							No;				//!<���b�Z�[�W�ԍ�
			ResultType							Result;			//!<���b�Z�[�W����
		};

		struct Result {
			double								Time;			//!<����
			double								ReTime;			//!<�đ�����
			uint32_t							ReCount;		//!<�đ���
			UdpSocketPtr						Socket;			//!<�\�P�b�g
			SocketAddr							Addr;			//!<�A�h���X
			void*								pBuffer;		//!<���b�Z�[�W�o�b�t�@
			uint32_t							BufSize;		//!<���b�Z�[�W�o�b�t�@�T�C�Y

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
			/** ��M�f�[�^ */
			char*									pData;
			/** ��M�w�b�_�[ */
			Header									header;

			~MessageData() {
				SAFE_DELETE_ARRAY(pData);
			}
		};
		using MessageDataPtr = std::shared_ptr<MessageData>;

		struct RUDPAddr {
			/** ��M�A�h���X */
			SocketAddr								addr;
			/** UDP���b�Z�[�W�ԍ� */
			uint32_t								MessageNo;
			/** RUDP���b�Z�[�W�ԍ� */
			uint32_t								RequestMessageNo;
			/** RUDP���b�Z�[�W��M�ԍ� */
			uint32_t								ReceiveNo;
			/** ��M�f�[�^ */
			std::list< MessageDataPtr >				MessageDatas;
			/** �ŏI�ʐM���� */
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
			 * @brief		�Y���ԍ��̃��b�Z�[�W���݊m�F
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
		/** �ڑ��A�h���X���Ƃ̑��M�Ǘ� */
		std::list< RUDPAddrPtr >				ConnectAddr_;
		/** �đ����s�� */
		uint32_t								RetryMax_;
		/** �A�h���X�폜�܂ł̎��� */
		double									DeleteTime;
		/** �đ��m�F���K�v�ȃf�[�^���X�g */
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
			//�����Ԗ��ʐM�̍폜
			ConnectAddr_.erase(std::remove_if(ConnectAddr_.begin(), ConnectAddr_.end(),
				[this, nt](const RUDPAddrPtr& it) {
					return (nt - it->lastTime) >= DeleteTime;
				}), ConnectAddr_.end());
			return TRUE;
		}

		/*
		 * @brief			�Ǘ��A�h���X�̎擾
		 * @param[in]		addr			�A�h���X
		 */
		RUDPAddrPtr GetAddr(const SocketAddr& addr)
		{
			//�A�h���X��񌟍�
			const auto& it = std::find_if(ConnectAddr_.begin(), ConnectAddr_.end(),
				[addr](const RUDPAddrPtr& data) {
					return data->addr.sin_addr.S_un.S_addr == addr.sin_addr.S_un.S_addr && data->addr.sin_port == addr.sin_port;
				});
			//���݂��Ȃ��ꍇ�V�K�쐬
			if (it == ConnectAddr_.end())
			{
				RUDPAddrPtr saddr = std::make_shared<RUDPAddr>(addr);
				ConnectAddr_.push_back(saddr);
				return saddr;
			}
			return *it;
		}

		/**
		 * @brief			�f�[�^�̑��M
		 * @param[in]		sock			���M�\�P�b�g
		 * @param[in]		addr			���M�A�h���X
		 * @param[in]		pData			���M�o�b�t�@
		 * @param[in]		Size			���M�o�b�t�@�T�C�Y
		 * @param[in]		type			���M���b�Z�[�W���ʎq
		 * @param[in]		ReTime			�đ��܂ł̎���
		 */
		bool Send(UdpSocketPtr& sock, const SocketAddr& addr, const void* pData, uint32_t Size, uint32_t type, double ReTime = 0) {
			//�A�h���X��񌟍�
			RUDPAddrPtr saddr = GetAddr(addr);
			saddr->lastTime = FrameTimer::Instance().NowTime();
			//��M�f�[�^�쐬
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
		 * @brief			�f�[�^�̑��M
		 * @param[in/out]	pData			��M�f�[�^
		 * @param[in]		Size			��M�f�[�^�T�C�Y
		 * @param[in/out]	oheader			��M���b�Z�[�W�w�b�_�[
		 * @param[in/out]	raddr			��M�A�h���X
		 * @param[in]		os				�ʐM�\�P�b�g
		 */
		bool Receive(void* pData, uint32_t Size, Header& oheader, SocketAddr& raddr, UdpSocketPtr& os) {
			//�����Ώۃf�[�^���c���Ă���Ȃ炻��������
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
			//��M�f�[�^�͂Ȃ�
			if (!os->IsReceive())
			{
				return false;
			}
			uint32_t rsize = os->Receive(pData, Size, raddr);
			char* buf = (char*)pData;
			memcpy(&oheader, buf, sizeof(Header));
			memcpy(buf, &buf[sizeof(Header)], oheader.Size);
			//�ԐM
			if (oheader.Result == ResultType::Request)
			{
				//��M�m�F�ԐM
				Header rehd = oheader;
				rehd.Size = 0;
				rehd.Result = ResultType::Succeeded;
				os->Send(raddr, &rehd, sizeof(Header));
				//�A�h���X��񌟍�
				RUDPAddrPtr saddr = GetAddr(raddr);
				saddr->lastTime = FrameTimer::Instance().NowTime();
				//�ԐM�v���̃��b�Z�[�W���X�g�b�N
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
				//�����ς݂̃f�[�^�̂��ߔj��
				else if (saddr->ReceiveNo > oheader.No)
				{
					return Receive(pData, Size, oheader, raddr, os);
				}
				//�����Ώۃf�[�^�Ȃ̂ňʒu��i�߂�
				else
				{
					saddr->ReceiveNo++;
				}
			}
			//�ԐM�m�F
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