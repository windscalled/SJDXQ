// Rename.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Rename.h"
#include "afxdialogex.h"


// CRename �Ի���

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


// CRename ��Ϣ�������


void CRename::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_EDIT1)->GetWindowText(theApp.m_rename) ;
	CDialog::OnOK();
}


BOOL CRename::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_EDIT1)->SetWindowText(theApp.ziyuan) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
