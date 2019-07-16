#pragma once
#include "stdafx.h"
#include "Head.h"

// CTcpClient 命令目标

class CTcpClient : public CSocket
{
public:
	CTcpClient(CPtrList* pList);
	virtual ~CTcpClient();
	virtual void OnClose(int nErrorCode) ;
	virtual void OnReceive(int nErrorCode) ;

	CPtrList* m_pList;     //保存服务器Socket中List的东西
	CString m_strID;       //连接账号
	CString m_strIDtemp;   //临时账号（注册，找密时用）
	CString m_strIp;       //连接IP
	int n_LoginErr ;       //登陆出错
	void sendmsg(CHead headobj);
};


