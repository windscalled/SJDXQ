#pragma once


// CRename �Ի���

class CRename : public CDialog
{
	DECLARE_DYNAMIC(CRename)

public:
	CRename(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRename();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
};
