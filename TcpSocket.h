#pragma once
#include "TcpClient.h"
// CTcpSocket ����Ŀ��

class CTcpSocket : public CSocket
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void OnAccept(int nErrorCode) ;
	CPtrList savesockets ;       //�����������ӿͻ���socket
};


