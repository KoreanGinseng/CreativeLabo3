#pragma once
#include "../Socket/MultiServer.h"
#include "../Socket/GameDataDefine.h"

// ********************************************************************************
/// <summary>
/// 
/// </summary>
// ********************************************************************************
class CMyServer : public CMultiServer
{
protected:



public:

    explicit CMyServer(Protocol prot, unsigned int multiCount = 10, int portNo = 18900, bool bStart = false);

	virtual void Recieve(const DataHeader& header, const void* data, int datalen) override;
};

