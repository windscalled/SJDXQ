#pragma once
#include "ReceiveSocket.h"
// CReceiveServer ����Ŀ��

class CReceiveServer : public CSocket
{
public:
	CReceiveServer();
	virtual ~CReceiveServer();
	virtual void OnAccept(int nErrorCode);
};


