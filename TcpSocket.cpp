// TcpSocket.cpp : ʵ���ļ�
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


// CTcpSocket ��Ա����
void CTcpSocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CTcpClient *m_client(0);
	m_client= new CTcpClient(&savesockets);
	Accept(*m_client);
	savesockets.AddTail(m_client);
	CSocket::OnAccept(nErrorCode);
	
}
