#pragma once
#include "stdafx.h"
#include "Head.h"

// CTcpClient ����Ŀ��

class CTcpClient : public CSocket
{
public:
	CTcpClient(CPtrList* pList);
	virtual ~CTcpClient();
	virtual void OnClose(int nErrorCode) ;
	virtual void OnReceive(int nErrorCode) ;

	CPtrList* m_pList;     //���������Socket��List�Ķ���
	CString m_strID;       //�����˺�
	CString m_strIDtemp;   //��ʱ�˺ţ�ע�ᣬ����ʱ�ã�
	CString m_strIp;       //����IP
	int n_LoginErr ;       //��½����
	void sendmsg(CHead headobj);
};


