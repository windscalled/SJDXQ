#pragma once
#include "ReceiveSocket.h"
// CReceiveServer ÃüÁîÄ¿±ê

class CReceiveServer : public CSocket
{
public:
	CReceiveServer();
	virtual ~CReceiveServer();
	virtual void OnAccept(int nErrorCode);
};


