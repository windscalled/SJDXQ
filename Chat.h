#pragma once
#include "afxwin.h"
#include "file_transfer.h"
#include "Chatrecord.h"


// CChat 对话框

class CChat : public CDialogEx
{
	DECLARE_DYNAMIC(CChat)

public:
	CChat(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChat();

// 对话框数据
	enum { IDD = IDD_CHAT };

	static BOOL GetErrMsg(CString& strOutErr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFile_transfer dlg ;
	CString mycaption;//标题
	CString mytoid;//聊天对象
	CString othername;
	virtual BOOL OnInitDialog();
	CEdit m_recmsgctr;
	CString m_recmessage;
	CString m_sendmsg;
	afx_msg void OnBnClickedsend();
	CImage m_image1;
	CBrush m_Br2;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedfile();
	afx_msg void OnBnClickedchatting();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString chatting_record ;
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
