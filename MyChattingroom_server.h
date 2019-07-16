
// MyChattingroom_server.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "MyChattingroom_serverDlg.h"
#include "Head.h"
#include <mysql.h>


// CMyChattingroom_serverApp:
// 有关此类的实现，请参阅 MyChattingroom_server.cpp


class CMyChattingroom_serverApp : public CWinApp
{
public:
	CMyChattingroom_serverApp();

	CMyChattingroom_serverDlg * active ;
	CHead head;
	map<CString,CString> m_clientMessage;//记录当前连接的所有客户端信息
	vector<CHead> m_user;//账号密码
	char file_time[200] ;
	char file_nicname[512] ;
	char file_state[200] ;
// 重写
public:
	MYSQL* mysql;
	virtual BOOL InitInstance(); 
	void my_mysql()
	{
		mysql = mysql_init(NULL);
		if (mysql == NULL)
		{
			AfxMessageBox("本地数据库初始化失败！");
		}

		my_bool reconnect = true;
		mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
		mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "gbk");

		if (!mysql_real_connect(mysql, NULL, "root", "980813", "chattingroom",	0, NULL, 0))
		{
			AfxMessageBox("打开数据库失败！");
		}

	}

// 实现
	DECLARE_MESSAGE_MAP()
};

extern CMyChattingroom_serverApp theApp;