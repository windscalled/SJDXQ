#pragma once


// CRegist �Ի���

class CRegist : public CDialog
{
	DECLARE_DYNAMIC(CRegist)

public:
	CRegist(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegist();

// �Ի�������
	enum { IDD = IDD_REGIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog() ;
	DECLARE_MESSAGE_MAP()
public:
	
	CString mynicname;
	CString mypassword;
	CString request;
	CString question;
	CString answer;
	afx_msg void OnBnClickedconfirm();
	afx_msg void OnBnClickedcancel();
	CImage m_image1;
	CBrush m_Br2;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CFont font ;
	afx_msg void OnPaint();
};
