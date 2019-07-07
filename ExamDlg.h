#if !defined(AFX_EXAMDLG_H__5A7A36F4_2E0F_40C4_A622_6658DF897394__INCLUDED_)
#define AFX_EXAMDLG_H__5A7A36F4_2E0F_40C4_A622_6658DF897394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

class CExamDlg : public CDialog
{
public:
	CExamDlg(CWnd* pParent = NULL);	


	enum { IDD = IDD_EXAM_DIALOG };
	CString	m_pUserName;
	CString	m_pPass;
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnReg();

	DECLARE_MESSAGE_MAP()
private:
	int OnLogin(CString userName, CString pass);
	BOOL AdoInit();

	char m_pProgPath[512];
	char m_pConnect[512];
	char m_pDBName[256];
	char m_pConnectString[1024];
	_RecordsetPtr m_pRst;
};



#endif 
