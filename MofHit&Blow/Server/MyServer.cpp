#include "MyServer.h"

CMyServer::CMyServer(Protocol prot, unsigned int multiCount, int portNo, bool bStart)
	: CMultiServer(prot, multiCount, portNo, bStart)
{
}

void CMyServer::Recieve(const DataHeader& header, const void* data, int datalen)
{
	switch ((GameDataType)header.Type)
	{
	case GameDataType::None:
		break;
	}
}
