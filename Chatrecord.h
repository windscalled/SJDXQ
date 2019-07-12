#pragma once


// CChatrecord 对话框

class CChatrecord : public CDialogEx
{
	DECLARE_DYNAMIC(CChatrecord)

public:
	CChatrecord(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CChatrecord();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };
	virtual BOOL OnInitDialog();
	CString m_caption ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CImage m_image1;
	CBrush m_Br2;
	CString myrecord;
	afx_msg void OnBnClickedButton1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
