#pragma once 


// CRename 对话框

class CRename : public CDialog
{
	DECLARE_DYNAMIC(CRename)

public:
	CRename(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRename();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
