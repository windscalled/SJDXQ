#pragma once


// CChatrecord �Ի���

class CChatrecord : public CDialogEx
{
	DECLARE_DYNAMIC(CChatrecord)

public:
	CChatrecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatrecord();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };
	virtual BOOL OnInitDialog();
	CString m_caption ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
