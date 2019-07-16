#pragma once
#include "TcpClient.h"
// CTcpSocket 命令目标

class CTcpSocket : public CSocket
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void OnAccept(int nErrorCode) ;
	CPtrList savesockets ;       //用来保存连接客户端socket
};


