// UdpSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "UdpSocket.h"


// CUdpSocket

CUdpSocket::CUdpSocket()
{
}

CUdpSocket::~CUdpSocket()
{
}


// CUdpSocket 成员函数
void CUdpSocket::OnReceive(int nErrorCode)//客户端监听端口1000，发送端口1001
{
	// TODO: 在此添加专用代码和/或调用基类
	char recBuf[1024] = {0};
	int len = sizeof(SOCKADDR_IN);
	int recBytes = ReceiveFrom(recBuf, 1023, (SOCKADDR*)&m_serverAddr, &len, 0);
	if(strcmp(recBuf,("QQ服务器应答")) == 0)
	{
		s = inet_ntoa(m_serverAddr.sin_addr);
		theApp.head.type = 0;
		theApp.search = 1;
	}
	CSocket::OnReceive(nErrorCode);
}

