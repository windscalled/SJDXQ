// Regist.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Regist.h"
#include "afxdialogex.h"


// CRegist 对话框

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


// CRegist 消息处理程序


void CRegist::OnBnClickedconfirm()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(mynicname == ""||mypassword == ""||request == ""||question == ""||answer == "")
	{
		return;
	}
		
	if(mypassword != request)
	{
		MessageBox(_T("对不起，您输入的密码不一致，请重新输入！"));
		return;
	}
	CDialog::OnOK();
}


void CRegist::OnBnClickedcancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//SendMessage(theApp.log->GetSafeHwnd(),WM_MSG,0);
	CDialog::OnCancel();
}


BOOL CRegist::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString strPath = TEXT("../../picture/4.bmp");
	if(m_image1.Load((LPCTSTR)strPath) != S_OK) //根据图片路径加载图片 
	{
		AfxMessageBox(TEXT("Load Error!"));
		return FALSE;
	}
	HBITMAP hBit = m_image1;
	CBitmap cBit;
	m_Br2.DeleteObject() ; //用来初始化
	CBitmap* pBit = cBit.FromHandle(hBit);
	m_Br2.CreatePatternBrush(pBit);
	cBit.DeleteObject();

	HBITMAP hbit_button1, hbit_button2;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/confirm.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/cancle.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	((CButton*)GetDlgItem(IDC_confirm))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_cancel))->SetBitmap(hbit_button2) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"宋体");
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

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;
		
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1||pWnd->GetDlgCtrlID() == IDC_STATIC2||pWnd->GetDlgCtrlID() == IDC_STATIC3||pWnd->GetDlgCtrlID() == IDC_STATIC4||pWnd->GetDlgCtrlID() == IDC_STATIC5)
	{
		pDC->SetBkMode(TRANSPARENT); //设置背景为透明
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}



void CRegist::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CRect rect ;
	CFont font;
	GetClientRect(&rect);     //取客户区大小 

	CString str("请用户正确填写注册信息") ;
	dc.SetTextColor(RGB(0,0,0)) ;
	font.CreatePointFont(260,(LPTSTR)"华文行楷");
	CFont *oldfont1= dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT) ;   //设置透明背景
	dc.TextOut(rect.left+90,rect.top+20,str) ;
	dc.SelectObject(oldfont1);
}
