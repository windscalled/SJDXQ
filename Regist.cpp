// Regist.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Regist.h"
#include "afxdialogex.h"


// CRegist �Ի���

IMPLEMENT_DYNAMIC(CRegist, CDialog)

CRegist::CRegist(CWnd* pParent /*=NULL*/)
	: CDialog(CRegist::IDD, pParent)
	, mynicname(_T(""))
	, mypassword(_T(""))
	, request(_T(""))
	, question(_T(""))
	, answer(_T(""))
{

}

CRegist::~CRegist()
{
}

void CRegist::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mynicname);
	DDX_Text(pDX, IDC_EDIT2, mypassword);
	DDX_Text(pDX, IDC_EDIT3, request);
	DDX_Text(pDX, IDC_EDIT4, question);
	DDX_Text(pDX, IDC_EDIT5, answer);
}


BEGIN_MESSAGE_MAP(CRegist, CDialog)
	ON_BN_CLICKED(IDC_confirm, &CRegist::OnBnClickedconfirm)
	ON_BN_CLICKED(IDC_cancel, &CRegist::OnBnClickedcancel)
	ON_WM_CTLCOLOR()

	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRegist ��Ϣ�������


void CRegist::OnBnClickedconfirm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	if(mynicname == ""||mypassword == ""||request == ""||question == ""||answer == "")
	{
		return;
	}
		
	if(mypassword != request)
	{
		MessageBox(_T("�Բ�������������벻һ�£����������룡"));
		return;
	}
	CDialog::OnOK();
}


void CRegist::OnBnClickedcancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//SendMessage(theApp.log->GetSafeHwnd(),WM_MSG,0);
	CDialog::OnCancel();
}


BOOL CRegist::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
	((CButton*)GetDlgItem(IDC_confirm))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_cancel))->SetBitmap(hbit_button2) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"����");
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC2)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC3)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC4)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC5)->SetFont(&font) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control

}



HBRUSH CRegist::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;
		
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1||pWnd->GetDlgCtrlID() == IDC_STATIC2||pWnd->GetDlgCtrlID() == IDC_STATIC3||pWnd->GetDlgCtrlID() == IDC_STATIC4||pWnd->GetDlgCtrlID() == IDC_STATIC5)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}



void CRegist::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CRect rect ;
	CFont font;
	GetClientRect(&rect);     //ȡ�ͻ�����С 

	CString str("���û���ȷ��дע����Ϣ") ;
	dc.SetTextColor(RGB(0,0,0)) ;
	font.CreatePointFont(260,(LPTSTR)"�����п�");
	CFont *oldfont1= dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT) ;   //����͸������
	dc.TextOut(rect.left+90,rect.top+20,str) ;
	dc.SelectObject(oldfont1);
}
