#pragma once

// CUdpSocket ����Ŀ��

class CUdpSocket : public CSocket
{
public:
	CUdpSocket();
	virtual ~CUdpSocket();
	virtual void OnReceive(int nErrorCode);
	SOCKADDR_IN m_serverAddr;
	CString s ;
};


