// ReceiveServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "ReceiveServer.h"


// CReceiveServer

CReceiveServer::CReceiveServer()
{
}

CReceiveServer::~CReceiveServer()
{
}


// CReceiveServer ��Ա����
void CReceiveServer::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CReceiveSocket* pSocket = new CReceiveSocket();
	Accept(*pSocket); 
	pSocket->OpenFile(theApp.recve->saveFilePath);	
	CSocket::OnAccept(nErrorCode);
}