#pragma once


// CQuestion 对话框

class CQuestion : public CDialogEx
{
	DECLARE_DYNAMIC(CQuestion)

public:
	CQuestion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CQuestion();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CString myanswer;
	afx_msg void OnBnClickedButton1();
	CImage m_image1;
	CBrush m_Br2;
	CFont font;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnCancel();

	virtual void PostNcDestroy();
};
