// ReceiveServer.cpp : 实现文件
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


// CReceiveServer 成员函数
void CReceiveServer::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CReceiveSocket* pSocket = new CReceiveSocket();
	Accept(*pSocket); 
	pSocket->OpenFile(theApp.recve->saveFilePath);	
	CSocket::OnAccept(nErrorCode);
}