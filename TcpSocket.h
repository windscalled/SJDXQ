#pragma once
#include "Head.h"
#include "Receivefile.h"

// CTcpSocket 命令目标

class CTcpSocket : public CSocket
{
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void OnReceive(int nErrorCode);
	void ChatUpdateMsg(CHead Msg);
	void UpdateListCtrl(CHead Msg);    //更新窗口列表

	CArchive* m_pArchiveOut;
	CArchive* m_pArchiveIn;
	CSocketFile* m_pFile;
	CFile file;//要保存的文件变量
	CReceivefile  dlg1 ;
	CString record ;
};



