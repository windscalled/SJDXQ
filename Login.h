#pragma once
#include "afxcmn.h"
#include "Regist.h"
#include "afxwin.h"
#include "Modify.h"


// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	
	CString mypassstr;
	int tcp_tag;
	afx_msg void OnBnClickedregist();
	afx_msg void OnBnClickeddenglu();
	void GetPath(CString &sPath,CString filename);
	virtual BOOL OnInitDialog();
	void Read();
	void Write();
	CString sPath;
	vector<string> vec;
	CComboBox myuserctr;
	CString myuserstr;	
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CImage m_image1;
	CBrush m_Br2;
	afx_msg void OnBnClickedpassword();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

