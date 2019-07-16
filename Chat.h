#pragma once
#include "afxwin.h"
#include "file_transfer.h"
#include "Chatrecord.h"


// CChat �Ի���

class CChat : public CDialogEx
{
	DECLARE_DYNAMIC(CChat)

public:
	CChat(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChat();

// �Ի�������
	enum { IDD = IDD_CHAT };

	static BOOL GetErrMsg(CString& strOutErr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFile_transfer dlg ;
	CString mycaption;//����
	CString mytoid;//�������
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
