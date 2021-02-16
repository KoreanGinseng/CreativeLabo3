#include "MyServer.h"

void CMyServer::Recieve(const GameSendData & data, int size)
{
	switch (data.DataType)
	{
	case GameDataType::None:
		break;
	}
}
