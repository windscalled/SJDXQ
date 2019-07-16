#pragma once

// CUdpSocket ÃüÁîÄ¿±ê

class CUdpSocket : public CSocket
{
public:
	CUdpSocket();
	virtual ~CUdpSocket();
	virtual void OnReceive(int nErrorCode) ;

	CString clientip;
	SOCKADDR_IN m_clientAddr;

};

