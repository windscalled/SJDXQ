#pragma once
#include "Head.h"
#include "Receivefile.h"

// CTcpSocket ����Ŀ��

class CTcpSocket : public CSocket
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void OnReceive(int nErrorCode);
	void ChatUpdateMsg(CHead Msg);
	void UpdateListCtrl(CHead Msg);    //���´����б�

	CArchive* m_pArchiveOut;
	CArchive* m_pArchiveIn;
	CSocketFile* m_pFile;
	CFile file;//Ҫ������ļ�����
	CReceivefile  dlg1 ;
	CString record ;
};



