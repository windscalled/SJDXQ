#if !defined(AFX_REGUSER_H__3B517E75_5192_4541_A5DC_523FF73FC510__INCLUDED_)
#define AFX_REGUSER_H__3B517E75_5192_4541_A5DC_523FF73FC510__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegUser dialog

class CRegUser : public CDialog
{
// Construction
public:
	CRegUser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegUser)
	enum { IDD = IDD_DLG_REG };
	CString	m_pUserName;
	CString	m_pPass;
	CString	m_pPassAg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegUser)
	afx_msg void OnBtnOk();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	char m_pProgPath[512];
	char m_pConnect[512];
	char m_pDBName[256];
	char m_pConnectString[1024];
	_RecordsetPtr m_pRst;
	_ConnectionPtr m_pConn;
	AdoInit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGUSER_H__3B517E75_5192_4541_A5DC_523FF73FC510__INCLUDED_)
