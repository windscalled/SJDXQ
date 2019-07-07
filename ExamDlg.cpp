// ExamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Exam.h"
#include "ExamDlg.h"
#include "RegUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExamDlg dialog

CExamDlg::CExamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExamDlg)
	m_pUserName = _T("");
	m_pPass = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExamDlg)
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_pUserName);
	DDX_Text(pDX, IDC_EDIT_PASS, m_pPass);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExamDlg, CDialog)
	//{{AFX_MSG_MAP(CExamDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_BTN_REG, OnBtnReg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExamDlg message handlers

BOOL CExamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	if(!AdoInit())
	{
		return FALSE;
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExamDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExamDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CExamDlg::OnBtnOk() 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	int iRet = OnLogin(m_pUserName,m_pPass);
	if(iRet == -1)
	{
		MessageBox("数据库并不存在");
	}
	else if(iRet == 0)
	{
		MessageBox("错误的用户名或密码");
	}
	else if(iRet == 1)
	{
		MessageBox("登陆成功");
	}
	else if(iRet > 1)
	{
		MessageBox("用重名用户");
	}
}

BOOL CExamDlg::AdoInit()
{
	if(!AfxOleInit())
	{
		MessageBox("OLE初始化失败");
		return FALSE;
	}
	memset(m_pConnectString,0x0,sizeof(m_pConnectString));
	memset(m_pProgPath,0x0,sizeof(m_pProgPath));
	GetCurrentDirectory(256,m_pProgPath);                           //获得程序路径
	m_pProgPath[strlen(m_pProgPath)]='\\';
	strcpy(m_pConnect,"DRIVER={Microsoft Access Driver (*.mdb)};DBQ=");
	strcpy(m_pDBName,"Exam.mdb");
	strcat(m_pConnectString,m_pConnect);
	strcat(m_pConnectString,m_pProgPath);
	strcat(m_pConnectString,m_pDBName);
	return TRUE;
}

int CExamDlg::OnLogin(CString userName, CString pass)
{
	if (FAILED(m_pRst.CreateInstance("ADODB.Recordset")))
	{
		MessageBox("Create Instance failed!");
		return FALSE;
	}

	int i = 0;
	CString sql = "select 1 from user_t where UserName='"+userName+"' and Password='"+pass+"'";
	if(FAILED(m_pRst->Open((_variant_t)sql,(_variant_t)m_pConnectString,adOpenStatic,adLockOptimistic,adCmdText)))
	{
		m_pRst.Release();
		return -1;
	}
	while(!m_pRst->GetadoEOF())
	{
		i++;
		m_pRst->MoveNext();
		
	}
	m_pRst.Release();
	return i;
}

void CExamDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}

void CExamDlg::OnBtnReg() 
{
	// TODO: Add your control notification handler code here
	CRegUser userDlg;
	userDlg.DoModal();
}
