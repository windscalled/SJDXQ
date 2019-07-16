
// MyChattingroom_server.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MyChattingroom_serverDlg.h"
#include "Head.h"
#include <mysql.h>


// CMyChattingroom_serverApp:
// �йش����ʵ�֣������ MyChattingroom_server.cpp


class CMyChattingroom_serverApp : public CWinApp
{
public:
	CMyChattingroom_serverApp();

	CMyChattingroom_serverDlg * active ;
	CHead head;
	map<CString,CString> m_clientMessage;//��¼��ǰ���ӵ����пͻ�����Ϣ
	vector<CHead> m_user;//�˺�����
	char file_time[200] ;
	char file_nicname[512] ;
	char file_state[200] ;
// ��д
public:
	MYSQL* mysql;
	virtual BOOL InitInstance(); 
	void my_mysql()
	{
		mysql = mysql_init(NULL);
		if (mysql == NULL)
		{
			AfxMessageBox("�������ݿ��ʼ��ʧ�ܣ�");
		}

		my_bool reconnect = true;
		mysql_options(mysql, MYSQL_OPT_RECONNECT, &reconnect);
		mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "gbk");

		if (!mysql_real_connect(mysql, NULL, "root", "980813", "chattingroom",	0, NULL, 0))
		{
			AfxMessageBox("�����ݿ�ʧ�ܣ�");
		}

	}

// ʵ��
	DECLARE_MESSAGE_MAP()
};

extern CMyChattingroom_serverApp theApp;