// UdpSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_server.h"
#include "UdpSocket.h"


// CUdpSocket

CUdpSocket::CUdpSocket()
{
}

CUdpSocket::~CUdpSocket()
{
}


// CUdpSocket ��Ա����

void CUdpSocket::OnReceive(int nErrorCode)//�����������˿�1001�����Ͷ˿�1000
{
	// TODO: �ڴ����ר�ô����/����û���
	char recBuf[1024] = {0};
	int len = sizeof(SOCKADDR_IN);
	int recBytes = ReceiveFrom(recBuf,1023,(SOCKADDR*)&m_clientAddr,&len,0);
	clientip = inet_ntoa(m_clientAddr.sin_addr);
	if(strcmp(recBuf,"QQ�ͻ��˲��ҷ�����") == 0)//���ַ����Ƚ�
	{
		CString str = _T("QQ������Ӧ��");
		SendTo(str,2*str.GetLength(),1000,clientip);
		//��������TCP��Ϣ
		theApp.head.type = 0;//��Ϣ������Ϊ0
		SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
	}
	CSocket::OnReceive(nErrorCode);
}
