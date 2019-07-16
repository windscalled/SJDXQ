// Rename.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Rename.h"
#include "afxdialogex.h"


// CRename 对话框

IMPLEMENT_DYNAMIC(CRename, CDialog)

CRename::CRename(CWnd* pParent /*=NULL*/)
	: CDialog(CRename::IDD, pParent)
{

}

CRename::~CRename()
{
}

void CRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRename, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CRename::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRename 消息处理程序


void CRename::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->GetWindowText(theApp.m_rename) ;
	CDialog::OnOK();
}


BOOL CRename::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT1)->SetWindowText(theApp.ziyuan) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
