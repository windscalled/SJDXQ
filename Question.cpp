// Question.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Question.h"
#include "afxdialogex.h"


// CQuestion �Ի���

IMPLEMENT_DYNAMIC(CQuestion, CDialogEx)

CQuestion::CQuestion(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQuestion::IDD, pParent)
	, myanswer(_T(""))
{
	
}

CQuestion::~CQuestion()
{
}

void CQuestion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myanswer);
}


BEGIN_MESSAGE_MAP(CQuestion, CDialogEx)
	ON_MESSAGE(WM_MSG,OnMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CQuestion::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CQuestion ��Ϣ�������


void CQuestion::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE) ;
	if(myanswer == "")
	{
		return;
	}
		
	CHead temp;
	CString only(theApp.mylocalip);
	only += "00002";
	temp.type = 7;		//�ѻش��͸�������
	strcpy_s(temp.message,(CStringA)only);
	strcpy_s(temp.answer,(CStringA)myanswer);
	strcpy(temp.id,(CStringA)theApp.m_userid);
	theApp.mytcp->Send(&temp,sizeof(temp));	
}

LRESULT CQuestion::OnMsg(WPARAM wParam,LPARAM lParam)
{
	if(theApp.modify_tag == 1)
	{
		theApp.m_password = (CString)"���˺ŵ�����Ϊ��"+ theApp.m_password ;
		MessageBox(theApp.m_password);
		theApp.modify_tag = 0 ;//����
		theApp.m_password = "";	
		OnCancel() ;	
		theApp.mod->OnCancel() ;
	}
	else
	{
		MessageBox(_T("�Բ������Ļش����"),_T("QQ"));
	}
	
	return TRUE;
}


BOOL CQuestion::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strPath = TEXT("../../picture/4.bmp");
	if(m_image1.Load((LPCTSTR)strPath) != S_OK) //����ͼƬ·������ͼƬ 
	{
		AfxMessageBox(TEXT("Load Error!"));
		return FALSE;
	}
	HBITMAP hBit = m_image1;
	CBitmap cBit;
	m_Br2.DeleteObject() ; //������ʼ��
	CBitmap* pBit = cBit.FromHandle(hBit);
	m_Br2.CreatePatternBrush(pBit);
	cBit.DeleteObject();

	HBITMAP hbit_button1, hbit_button2;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/confirm.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/cancle.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;
	GetDlgItem(IDC_question)->SetWindowText(theApp.m_question);

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"����");
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC2)->SetFont(&font) ;
	GetDlgItem(IDC_question)->SetFont(&font) ;
	return TRUE ;
}

HBRUSH CQuestion::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd == this)
	{
		return m_Br2 ;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1||pWnd->GetDlgCtrlID() == IDC_STATIC2||pWnd->GetDlgCtrlID() == IDC_question)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CQuestion::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	DestroyWindow() ;
}



void CQuestion::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::PostNcDestroy();
	theApp.mod->ShowWindow(SW_SHOW) ;
	delete theApp.ques ;
	theApp.ques = NULL ;
}
