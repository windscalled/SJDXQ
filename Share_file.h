#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "afxinet.h"
#include "Rename.h"

// CShare_file 对话框

class CShare_file : public CDialog
{
	DECLARE_DYNAMIC(CShare_file)

public:
	CShare_file(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShare_file();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CImage m_image1;
	CBrush m_Br2;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void ConnectFtp();
	void UpdateDir();
	CInternetSession *pInternetSession;
	CFtpConnection *pFtpConnection;
	CListBox m_listBox;
	CButton m_back;
	CButton m_cancle ;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton1();
	BOOL bFlag ;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnPaint();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnBnClickedButton8();
	CRename re_name ;
	afx_msg void OnBnClickedButton4();
	TCHAR szErr[1024];
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
