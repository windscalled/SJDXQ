// file_transfer.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "file_transfer.h"
#include "afxdialogex.h"


// file_transfer 对话框

IMPLEMENT_DYNAMIC(CFile_transfer, CDialogEx)

CFile_transfer::CFile_transfer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFile_transfer::IDD, pParent)
{
	theApp.sendFile = this ;
}

CFile_transfer::~CFile_transfer()
{
}

void CFile_transfer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, filepath);
	DDX_Control(pDX, IDC_BUTTON3, cancelsend);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}


BEGIN_MESSAGE_MAP(CFile_transfer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFile_transfer::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFile_transfer::OnBnClickedButton2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON3, &CFile_transfer::OnBnClickedButton3)
END_MESSAGE_MAP()


// file_transfer 消息处理程序




void CFile_transfer::OnBnClickedButton1()   //选择文件
{
	// TODO: 在此添加控件通知处理程序代码

	
	CFileDialog CFDlg(TRUE); 

	if (IDOK ==CFDlg.DoModal())     //打开文件成功
	{
		//取得文件全路径且显示到编辑框
		filepath.SetWindowText(CFDlg.GetPathName());

		//取得文件名显示到IDC_STATIC
		SetDlgItemText(IDC_STATIC_name,_T("文件名: ")+CFDlg.GetFileName());

		CFile fileTemp;
		if(fileTemp.Open(CFDlg.GetPathName(),CFile::modeRead|CFile::shareDenyNone))
		{
			CString filesize;
			filesize.Format(_T("%d kb"),fileTemp.GetLength()/1024);
			SetDlgItemText(IDC_STATIC_size,filesize);
		}
		else
		{
			SetDlgItemText(IDC_STATIC_size,_T("未知"));
		}
		fileTemp.Close();
	}
	Invalidate() ;
}


void CFile_transfer::OnBnClickedButton2()   //发送文件
{
	// TODO: 在此添加控件通知处理程序代码
	bStopSend = FALSE; //恢复,表示不中断发送线程

	CString m_strPath=_T("");
	filepath.GetWindowText(m_strPath);
	if (m_strPath=="")
	{
		AfxMessageBox(_T("请选择要发送的文件!"));
		return;
	}
	
	//取得发送文件大小.名称等
	CFile file;
	if(!file.Open(m_strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("文件打开失败")); 
		return;
	}
	DWORD m_dwFileSize = file.GetLength();    //文件长度
	//CMessage *pMsg  = new CMessage(REQUEST,file.GetFileName(),m_dwFileSize);
	

	CHead myHead ;
	myHead.type = 8;		//发送文件标志
	myHead.m_dwFileSize = m_dwFileSize ;
	strcpy_s(myHead.message,(CStringA)file.GetFileName()) ;
	strcpy_s(myHead.IP,(CStringA)theApp.mylocalip);
	strcpy_s(myHead.nicname,(CStringA)theApp.m_username);
	strcpy(myHead.fromID,(CStringA)theApp.m_userid);
	strcpy(myHead.toID,(CStringA)theApp.friendid);
	file.Close();
	//theApp.mytcp->SendMsg(pMsg) ;
	theApp.mytcp->Send(&myHead,sizeof(myHead));//请求发送文件
		
}


//发送数据线程
UINT _SendThread(LPVOID lparam)
{  	
	CFile_transfer* pDlg=(CFile_transfer*)lparam;//取得主窗口指针
	//建立新的Socket连接接收客户端接收线程创建的Socket
	//初始化Csocket,在线程中使用CSocket必须要用此句.否则在静态MFC编译运行时不能成功创建socket 
	AfxSocketInit();
	CSocket sendSocket;
	
	if (!sendSocket.Create())
	{
		AfxMessageBox(_T("sendSocket.Create error"));
		return -1;
	}
	
	if (!sendSocket.Connect(theApp.otherlocalip,1005))
	{
		AfxMessageBox(_T("sendSocket.Connect error"));
		return -1;
	}
	//可以发送文件了

	//取得文件 
	CString m_strPath=_T("");
	pDlg->filepath.GetWindowText(m_strPath);

	//打开要发送的文件
	CFile file;
	if(!file.Open(m_strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("文件打开失败")); 
		sendSocket.Close();
		return 1;
	}
	DWORD m_dwFileSize = file.GetLength();
	pDlg->m_ctrlProgress.SetRange32(0,m_dwFileSize);

	int nSize = 0, nLen = 0;
	DWORD dwCount = 0;
	char buf[512] = {0};
	file.Seek(0, CFile::begin);  
	//打开取消发送按钮为可用
	pDlg->cancelsend.EnableWindow(TRUE);
	//开始传送文件
	for(;;)
	{    

		//一次读取BLOCKSIZE大小的文件内容
		nLen = file.Read(buf, 512);
		if(nLen == 0)
			break;

		//发送文件内容
		nSize =sendSocket.Send(buf, nLen);

		dwCount += nSize;
		pDlg->m_ctrlProgress.SetPos(dwCount); 
		pDlg->SetDlgItemInt(IDC_STATIC_size,dwCount/1024);

		if (pDlg->bStopSend)//停止发送
		{
			pDlg->Invalidate(TRUE) ;
			strcpy(buf,"cancle_send!");
			sendSocket.Send(buf, sizeof(buf));
			break;
		}
	}
	//关闭文件
	file.Close();
	sendSocket.Close();

	if(m_dwFileSize == dwCount)
	{
		pDlg->Invalidate(TRUE) ;
		AfxMessageBox(_T("文件发送成功"));
	}
	else
	{
		AfxMessageBox(_T("文件发送失败"));
	}
	//发送进度归零
	pDlg->SetDlgItemInt(IDC_STATIC_size,0);
	pDlg->m_ctrlProgress.SetPos(0); 
	//打开取消发送按钮为不可用
	pDlg->cancelsend.EnableWindow(FALSE);


	return 0;
}



void CFile_transfer::sendFile()
{
	//启动发送文件线程
	AfxBeginThread(_SendThread,this);
}


BOOL CFile_transfer::OnInitDialog()
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

	HBITMAP hbit_button1, hbit_button2,hbit_button3;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/surf.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/send.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/cancle_send.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hbit_button3) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"宋体");
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_name)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_size)->SetFont(&font) ;

	cancelsend.EnableWindow(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CFile_transfer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1||pWnd->GetDlgCtrlID() == IDC_STATIC_name||pWnd->GetDlgCtrlID() == IDC_STATIC_size)
	{
		pDC->SetBkMode(TRANSPARENT); //设置背景为透明
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CFile_transfer::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	bStopSend = TRUE ;
}
