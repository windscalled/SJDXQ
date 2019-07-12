// Modify.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Modify.h"
#include "afxdialogex.h"


// CModify 对话框

IMPLEMENT_DYNAMIC(CModify, CDialogEx)

CModify::CModify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CModify::IDD, pParent)
	, findid(_T(""))
{
	
}

CModify::~CModify()
{
}

void CModify::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, findid);
}


BEGIN_MESSAGE_MAP(CModify, CDialogEx)
	ON_MESSAGE(WM_MSG,OnMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CModify::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnCancel)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CModify 消息处理程序


void CModify::OnBnClickedButton1()   //确定按钮
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if(findid == "")
	{
		return;
	}		
	else if(findid.GetLength()!=6)
	{
		MessageBox(_T("对不起，您的账号输入有误！"));
		return;
	}

	if(theApp.search != 1)
	{
		CString st("QQ客户端查找服务器");
		theApp.myudp->SendTo(st,2*st.GetLength(),1001,_T("255.255.255.255"));//发送广播
		MessageBox(_T("未搜索到服务器！"));
		return;
	}

	theApp.sereverip = theApp.myudp->s;//服务器IP
	theApp.mytcp = new CTcpSocket();
	if(!theApp.mytcp)
	{
		MessageBox(_T("内存不足！"));
		return;
	}
	else if(!theApp.mytcp->Create())
	{

		MessageBox(_T("创建套接字失败！"));

		return;
	}
	else if(!theApp.mytcp->Connect(theApp.sereverip,1003)) 
	{		
		MessageBox(_T("连接服务器失败?"));
		return;
	}
	//theApp.mytcp->Init();

	CHead temp;
	CString only(theApp.mylocalip);
	only += "00001";
	temp.type = 6;		//把账号发送给服务器
	theApp.m_userid = findid ;
	strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
	strcpy_s(temp.message,(CStringA)only);
	strcpy(temp.id,(CStringA)findid);
	theApp.mytcp->Send(&temp,sizeof(temp));

	
}



LRESULT CModify::OnMsg(WPARAM wParam,LPARAM lParam)
{
	if(theApp.modify_tag == 1)
	{	
		theApp.modify_tag = 0 ;//重置赋初值
		theApp.ques = new CQuestion() ;
		theApp.ques->Create(IDD_DIALOG3,NULL);
		theApp.ques->ShowWindow(SW_SHOW) ;
		theApp.mod->ShowWindow(SW_HIDE) ;				
	}
	else
	{
		MessageBox(_T("对不起，该账号尚未注册，请重新输入！"),_T("QQ"));
	}

	return TRUE;
}


BOOL CModify::OnInitDialog()
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

	HBITMAP hbit_button1, hbit_button2;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/confirm.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/cancle.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"宋体");
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CModify::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;

	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		pDC->SetBkMode(TRANSPARENT); //设置背景为透明
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CModify::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	DestroyWindow() ;
}


void CModify::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PostNcDestroy();

	delete theApp.mod ;
	theApp.mod = NULL ;
}
