#pragma once
#include "Question.h"
// CModify 对话框

class CModify : public CDialogEx
{
	DECLARE_DYNAMIC(CModify)

public:
	CModify(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModify();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CString findid;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	CImage m_image1;
	CBrush m_Br2;
	CFont font;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
