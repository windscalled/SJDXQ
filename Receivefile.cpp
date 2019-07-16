// Receivefile.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Receivefile.h"
#include "afxdialogex.h"


// CReceivefile 对话框

IMPLEMENT_DYNAMIC(CReceivefile, CDialogEx)

CReceivefile::CReceivefile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReceivefile::IDD, pParent)
{
	theApp.recve = this ;

	//文件大小
	m_dwFileSize = 0;
	//要保存的文件
	saveFilePath = TEXT("");
	saveFileFolder = TEXT("");
}

CReceivefile::~CReceivefile()
{
}

void CReceivefile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, progress1);
	DDX_Control(pDX, IDC_BUTTON2, m_openfile);
}



BEGIN_MESSAGE_MAP(CReceivefile, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CReceivefile::OnBnClickedButton2)
END_MESSAGE_MAP()


// CReceivefile 消息处理程序
//创建接收文件CSocket
void CReceivefile::CreateReceiveSoket()
{ 
	//TODO 创建接收文件CSocket
	theApp.serverSoc = new CReceiveServer();//接收文件服务端
	if(!theApp.serverSoc)
	{
		MessageBox(_T("内存不足！"));
		return;
	}

	if (!theApp.serverSoc->Create(1005,SOCK_STREAM,theApp.mylocalip))
	{
		AfxMessageBox(_T("接收文件时创建serverSoc错误!"));

	}
	if (!theApp.serverSoc->Listen(1))
	{
		AfxMessageBox(_T(" serverSoc监听错误!"));

	}

}

HBRUSH CReceivefile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_road||pWnd->GetDlgCtrlID() == IDC_STATIC_nam ||pWnd->GetDlgCtrlID() == IDC_STATIC_sizer||pWnd->GetDlgCtrlID() == IDC_RECV
		||pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		pDC->SetBkMode(TRANSPARENT); //设置背景为透明
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CReceivefile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	HBITMAP hbit_button2;
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/open_file.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"宋体");
	GetDlgItem(IDC_STATIC_road)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_nam)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_sizer)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;
	GetDlgItem(IDC_RECV)->SetFont(&font) ;

	m_openfile.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CReceivefile::OnBnClickedButton2()  //打开文件目录
{
	// TODO: 在此添加控件通知处理程序代码
	if (this->saveFileFolder!="")
	{ 
		ShellExecute(NULL,_T("explore"),saveFileFolder, NULL, NULL, SW_SHOW);
	}
}


