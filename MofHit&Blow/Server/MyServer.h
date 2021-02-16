#pragma once
#include "../TCPSocket/TCPMultiServer.h"
#include "../TCPSocket/GameDataDefine.h"

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
class CMyServer : public CTCPMultiServer<GameSendData>
{
protected:



public:

	virtual void Recieve(const GameSendData& data, int size) override;
};

