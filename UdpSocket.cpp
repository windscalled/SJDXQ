// UdpSocket.cpp : ʵ���ļ�
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


// CUdpSocket ��Ա����
void CUdpSocket::OnReceive(int nErrorCode)//�ͻ��˼����˿�1000�����Ͷ˿�1001
{
	// TODO: �ڴ����ר�ô����/����û���
	char recBuf[1024] = {0};
	int len = sizeof(SOCKADDR_IN);
	int recBytes = ReceiveFrom(recBuf, 1023, (SOCKADDR*)&m_serverAddr, &len, 0);
	if(strcmp(recBuf,("QQ������Ӧ��")) == 0)
	{
		s = inet_ntoa(m_serverAddr.sin_addr);
		theApp.head.type = 0;
		theApp.search = 1;
	}
	CSocket::OnReceive(nErrorCode);
}

