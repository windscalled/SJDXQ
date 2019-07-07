#include "stdafx.h"
#include "Exam.h"
#include "RegUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CRegUser::CRegUser(CWnd* pParent )
	: CDialog(CRegUser::IDD, pParent)
{
	m_pUserName = _T("");
	m_pPass = _T("");
	m_pPassAg = _T("");
}


void CRegUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_pUserName);
	DDX_Text(pDX, IDC_EDIT_PASS, m_pPass);
	DDX_Text(pDX, IDC_EDIT_PASS_AG, m_pPassAg);
}


BEGIN_MESSAGE_MAP(CRegUser, CDialog)
ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
END_MESSAGE_MAP()


void CRegUser::OnBtnOk() 
{
	UpdateData(TRUE);

	if(m_pUserName == "")
	{
		AfxMessageBox("请输入用户名");
		return;
	}
	if(m_pPassAg == "" || m_pPass == "" || m_pPass != m_pPassAg)
	{
		AfxMessageBox("两次输入的密码不一致");
		return;
	}
	if (FAILED(m_pConn.CreateInstance("ADODB.Connection")))
	{
		AfxMessageBox("Create Instance failed!");
		return;
	}
	if (FAILED(m_pConn->Open(m_pConnectString,"","",-1)))
	{
		AfxMessageBox("Can not open Database!");
		m_pConn.Release();
		return;
	}
	COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
	CString sql = "insert into user_t(UserName,Password) values('"+m_pUserName+"','"+m_pPass+"')";
	m_pConn->Execute(_bstr_t(sql),&vtOptional,-1);
	m_pConn->Close();
	m_pConn.Release();
}

BOOL CRegUser::AdoInit()
{
	memset(m_pConnectString,0x0,sizeof(m_pConnectString));
	memset(m_pProgPath,0x0,sizeof(m_pProgPath));
	GetCurrentDirectory(256,m_pProgPath);                           
	m_pProgPath[strlen(m_pProgPath)]='\\';
	strcpy(m_pConnect,"DRIVER={Microsoft Access Driver (*.mdb)};DBQ=");
	strcpy(m_pDBName,"Exam.mdb");
	strcat(m_pConnectString,m_pConnect);
	strcat(m_pConnectString,m_pProgPath);
	strcat(m_pConnectString,m_pDBName);
	return TRUE;
}

BOOL CRegUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!AdoInit())
	{
		return FALSE;
	}
	return TRUE; 
}
