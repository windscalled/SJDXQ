// TcpSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_server.h"
#include "TcpSocket.h"


// CTcpSocket

CTcpSocket::CTcpSocket()
{
}

CTcpSocket::~CTcpSocket()
{
}


// CTcpSocket 成员函数
void CTcpSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CTcpClient *m_client(0);
	m_client= new CTcpClient(&savesockets);
	Accept(*m_client);
	savesockets.AddTail(m_client);
	CSocket::OnAccept(nErrorCode);
	
}
