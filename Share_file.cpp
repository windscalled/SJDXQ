// Share_file.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Share_file.h"
#include "afxdialogex.h"


// CShare_file 对话框

IMPLEMENT_DYNAMIC(CShare_file, CDialog)

CShare_file::CShare_file(CWnd* pParent /*=NULL*/)
	: CDialog(CShare_file::IDD, pParent)
{

}

CShare_file::~CShare_file()
{
}

void CShare_file::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_BUTTON5, m_back);
	//DDX_Control(pDX, IDCANCEL, m_cancle);
}


BEGIN_MESSAGE_MAP(CShare_file, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON5, &CShare_file::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON1, &CShare_file::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CShare_file::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CShare_file::OnBnClickedButton2)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_LIST1, &CShare_file::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON8, &CShare_file::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON4, &CShare_file::OnBnClickedButton4)
END_MESSAGE_MAP()


// CShare_file 消息处理程序


BOOL CShare_file::OnInitDialog()
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

	bFlag = FALSE ;
	HBITMAP hbit_button1, hbit_button2,hbit_button3,hbit_button4,hbit_button5,hbit_button6,hbit_button7,hbit_button8;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/up.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/down.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/delete.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button4 = (HBITMAP)::LoadImage(NULL,"../../picture/exit (2).bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button5 = (HBITMAP)::LoadImage(NULL,"../../picture/new_file.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button6 = (HBITMAP)::LoadImage(NULL,"../../picture/return_f.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button8 = (HBITMAP)::LoadImage(NULL,"../../picture/re_name.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hbit_button3) ;
	((CButton*)GetDlgItem(IDC_BUTTON6))->SetBitmap(hbit_button4) ;
	((CButton*)GetDlgItem(IDC_BUTTON5))->SetBitmap(hbit_button6) ;
	((CButton*)GetDlgItem(IDC_BUTTON4))->SetBitmap(hbit_button5) ;
	((CButton*)GetDlgItem(IDC_BUTTON8))->SetBitmap(hbit_button8) ;

	ConnectFtp() ;
	if(!bFlag)
	{
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON6))->EnableWindow(TRUE) ;
		((CButton*)GetDlgItem(IDC_BUTTON5))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE) ;
		((CButton*)GetDlgItem(IDC_BUTTON8))->EnableWindow(FALSE) ;
		AfxMessageBox((CString)szErr+"   请退出！");
		return FALSE ;
	}

	UpdateDir() ;
	m_back.EnableWindow(false);

	

	//m_cancle.EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CShare_file::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CShare_file::ConnectFtp()  //连接存储服务器
{
	
	CString Ftp_IP = theApp.sereverip ;    //ftp服务器IP地址
	int nPort = 21 ;                     //端口号
	CString str_user = "anonymous" ;     //匿名
	CString str_pass = "" ;              //密码为空

	if (Ftp_IP.IsEmpty())
	{
		AfxMessageBox(_T("IP地址为空！"));
		return;
	}
	if (str_user.IsEmpty())
	{
		return;
	}
	//建立一个Internet会话
	pInternetSession= new CInternetSession("MR",INTERNET_OPEN_TYPE_PRECONFIG);

	try
	{
		//利用Internet会话对象pInternetSession打开一个FTP连接
		pFtpConnection=pInternetSession->GetFtpConnection(Ftp_IP,str_user,str_pass,nPort);
		bFlag = TRUE ;
	}
	catch (CInternetException* pEx)
	{
		pEx->GetErrorMessage(szErr,1024);
		
		pEx->Delete();
	}

}


void CShare_file::OnBnClickedButton5()   //返回上一层
{
	// TODO: 在此添加控件通知处理程序代码
	CString strdir;
	pFtpConnection->GetCurrentDirectory(strdir);

	int pos;
	//用字符串截取的方法获得上一级目录
	pos=strdir.ReverseFind('/');
	strdir=strdir.Left(pos);

	pInternetSession->Close();//关闭废弃的对话
	this->ConnectFtp();//重新连接保持持续会话
	pFtpConnection->SetCurrentDirectory(strdir);
	theApp.str_LoadPath = strdir ;
	this->UpdateDir();//更新目录列表

}


void CShare_file::OnBnClickedButton1()   //上传资源
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CString strname;
	//弹出“打开”对话框
	CFileDialog file(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"所有文件(*.*)|*.*|",this);
	if (file.DoModal()==IDOK)
	{
		str=file.GetPathName();
		strname=file.GetFileName();
	}
	if (bFlag)   //已连接
	{
		CString strdir;
		pFtpConnection->GetCurrentDirectory(strdir);
		//上传文件
		BOOL bput=pFtpConnection->PutFile((LPCTSTR)str,(LPCTSTR)strname);
		if (bput)
		{
			pInternetSession->Close();//关闭会话
			this->ConnectFtp();//重新连接保持持续会话
			pFtpConnection->SetCurrentDirectory(strdir);
			this->UpdateDir();//更新目录列表

			//发送上传资源信息
			CTime time=time.GetCurrentTime();
			CString str=time.Format("%Y-%m-%d %X");
			CString str2 = "上传文件："+strname ;
			CHead temp;  
			strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
			strcpy(temp.id,(CStringA)theApp.m_userid);
			strcpy_s(temp.password,(CStringA)"");
			strcpy(temp.fromID,(CStringA)"");
			temp.type = 10;
			strcpy(temp.message,str) ;
			strcpy_s(temp.nicname,(CStringA)theApp.m_username);
			strcpy_s(temp.question,str2);
			theApp.mytcp->Send(&temp,sizeof(temp));

			AfxMessageBox(_T("上传成功！"));
		}
	}
}


void CShare_file::OnBnClickedButton2()   //下载
{
	// TODO: 在此添加控件通知处理程序代码
	CString selfile;
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);//获取用户选择要下载的资源名
	selfile = selfile.Right(selfile.GetLength()-13);
	if (!selfile.IsEmpty())
	{
		//弹出“另存为”对话框
		CFileDialog file(FALSE,NULL,selfile,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"所有文件(*.*)|*.*|",this);
		if (file.DoModal()==IDOK)
		{
			CString strname;
			CString strpath ;
			strname=file.GetFileName();
			strpath = file.GetPathName() ;
			CString strdir;
			pFtpConnection->GetCurrentDirectory(strdir);
			pFtpConnection->GetFile(strname,strpath);//下载文件到选定的本地位置
			pInternetSession->Close();//关闭废弃的对话
			this->ConnectFtp();//保持持续会话
			pFtpConnection->SetCurrentDirectory(strdir);
			this->UpdateDir();//更新目录列表

			//发送下载资源信息
			CTime time=time.GetCurrentTime();
			CString str=time.Format("%Y-%m-%d %X");
			CString str2 = "下载文件："+strname ;
			CHead temp;  
			strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
			strcpy(temp.id,(CStringA)theApp.m_userid);
			strcpy_s(temp.password,(CStringA)"");
			strcpy(temp.fromID,(CStringA)"");
			temp.type = 10;
			strcpy(temp.message,str) ;
			strcpy_s(temp.nicname,(CStringA)theApp.m_username);
			strcpy_s(temp.question,str2);
			theApp.mytcp->Send(&temp,sizeof(temp));

			AfxMessageBox(_T("下载成功！"));
		}
	}
}


void CShare_file::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	CRect rect ;
	CFont font;
	GetClientRect(&rect);     //取客户区大小 

	CString str("浏 览 共 享 文 件") ;
	dc.SetTextColor(RGB(0,0,0)) ;
	font.CreatePointFont(280,(LPTSTR)"华文行楷");
	CFont *oldfont1= dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT) ;   //设置透明背景
	dc.TextOut(rect.left+130,rect.top+20,str) ;
	dc.SelectObject(oldfont1);
}


void CShare_file::UpdateDir()   //更新窗口
{
	m_listBox.ResetContent();  //清空组合框内容

	//读写服务器中的数据，需要创建一个CFtpFileFind的实例
	CFtpFileFind ftpfind(pFtpConnection);

	//找到第一个文件或者文件夹，通过CFtpFileFind::FindFile实现
	BOOL bfind=ftpfind.FindFile(NULL);
	while (bfind)
	{
		bfind=ftpfind.FindNextFile();
		CString strname;
		if (!ftpfind.IsDirectory())//判断是目录还是文件夹
		{
			strname=ftpfind.GetFileName();//是文件则读取文件名
			m_listBox.AddString("文件         "+strname);
		}
		else
		{
			strname=ftpfind.GetFileName();//如果是文件夹则读取文件名
			m_listBox.AddString("文件夹     "+strname);
		}
	}
}

void CShare_file::OnLbnDblclkList1()   //打开文件夹
{
	// TODO: 在此添加控件通知处理程序代码
	CString selfile,str;
	// 获取用户选择的目录名
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);

	if((str = selfile.Left(13)) == "文件         ")
	{
		return ;
	}

	selfile = selfile.Right(selfile.GetLength()-11);

	if (!selfile.IsEmpty())
	{
		pFtpConnection->Close();   //及时关闭废弃的会话句柄
		this->ConnectFtp();        //重新连接，保持与服务器的持续会话
		CString strdir;
		pFtpConnection->GetCurrentDirectory(strdir);//获得原来的工作目录
		strdir+=selfile;//生成新的目录
		theApp.str_LoadPath = theApp.str_LoadPath+ strdir ;
		pFtpConnection->SetCurrentDirectory(theApp.str_LoadPath); //改变目录到当前服务目录
		this->UpdateDir();         //更新目录列表

		m_back.EnableWindow(TRUE);
	}
}


void CShare_file::OnBnClickedButton8()   //重命名文件
{
	// TODO: 在此添加控件通知处理程序代码
	CString selfile;
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);//获取资源名
	if(selfile.Find("文件夹     ") == -1)
	{
		selfile = selfile.Right(selfile.GetLength()-13);
	}
	else
	{
		selfile = selfile.Right(selfile.GetLength()-11);
	}
	
	theApp.ziyuan = selfile ;
	if (!selfile.IsEmpty())
	{
		if(re_name.DoModal()!=IDOK)
		{
			return ;
		}
		theApp.ziyuan = "";
		pFtpConnection->Rename(selfile,theApp.m_rename);//重命名
		
		CString strdir;
		pFtpConnection->GetCurrentDirectory(strdir);
		pInternetSession->Close();//关闭废弃的会话
		this->ConnectFtp();//保持持续会话
		pFtpConnection->SetCurrentDirectory(strdir);
		this->UpdateDir();//更新目录列表
	}
}


void CShare_file::OnBnClickedButton4()  //新建文件夹
{
	// TODO: 在此添加控件通知处理程序代码
	if(re_name.DoModal()!=IDOK)
	{
		return ;
	}

	int flag = pFtpConnection->CreateDirectory(theApp.m_rename);  
	if (!flag)  
	{  
		AfxMessageBox("创建文件夹失败！");
	} 
	CString strdir;
	pFtpConnection->GetCurrentDirectory(strdir);
	pInternetSession->Close();//关闭废弃的会话
	this->ConnectFtp();//保持持续会话
	pFtpConnection->SetCurrentDirectory(strdir);
	this->UpdateDir();//更新目录列表
}


void CShare_file::OnBnClickedButton3()  //删除文件
{
	// TODO: 在此添加控件通知处理程序代码
	CString selfile;
	CString str2 ;
	m_listBox.GetText(m_listBox.GetCurSel(),selfile);//获取用户要删除的资源名

	if(selfile.Find("文件夹     ") == -1)
	{
		selfile = selfile.Right(selfile.GetLength()-13);
		if (!selfile.IsEmpty())
		{
			//弹出删除警告框
			if (AfxMessageBox("确定要删除这个文件？",4+48)==6)
			{
				pFtpConnection->Remove(selfile);//删除该文件				
				str2 = "删除文件："+selfile ;				
			}
		}
	}
	else
	{
		selfile = selfile.Right(selfile.GetLength()-11);
		if (!selfile.IsEmpty())
		{
			//弹出删除警告框
			if (AfxMessageBox("确定删除该文件夹？",4+48)==6)
			{
				int flag = pFtpConnection->RemoveDirectory(selfile);  
				if (!flag)  
				{  
					AfxMessageBox("删除失败，文件夹必须为空！");
				} 
				str2 = "删除文件夹："+selfile ;
				
			}
		}
	}
	//发送删除资源信息
	CTime time=time.GetCurrentTime();
	CString str=time.Format("%Y-%m-%d %X");
	CHead temp;  
	strcpy_s(temp.IP,(CStringA)theApp.mylocalip);
	strcpy(temp.id,(CStringA)theApp.m_userid);
	strcpy_s(temp.password,(CStringA)"");
	strcpy(temp.fromID,(CStringA)"");
	temp.type = 10;
	strcpy(temp.message,str) ;
	strcpy_s(temp.nicname,(CStringA)theApp.m_username);
	strcpy_s(temp.question,str2);
	theApp.mytcp->Send(&temp,sizeof(temp));

	CString strdir;
	pFtpConnection->GetCurrentDirectory(strdir);
	pInternetSession->Close();//关闭废弃的会话
	this->ConnectFtp();//保持持续会话
	pFtpConnection->SetCurrentDirectory(strdir);
	this->UpdateDir();//更新目录列表
	
}


//void CShare_file::OnBnClickedButton9()  //搜索文件
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CFtpFileFind fFinder(pFtpConnection);  
//	if(re_name.DoModal()!=IDOK)
//	{
//		return ;
//	}
//
//	BOOL bFind = fFinder.FindFile(theApp.m_rename);  
//	while (bFind)  
//	{  
//		bFind = fFinder.FindNextFile();  
//
//		//当前文件夹及上层文件夹(名称分别为.和..)
//		if (fFinder.IsDots())   
//		{  
//			continue;  
//		}  
//
//		//子文件夹
//		if(fFinder.IsDirectory())   
//		{  
//			CString cstrDirName = fFinder.GetFileName();  
//			CString cstrDirPath = fFinder.GetFilePath(); 
//			continue;  
//		}  
//
//		//文件
//		CString cstrFileName = fFinder.GetFileName();    
//		CString cstrFilePath = fFinder.GetFilePath(); 
//
//		pInternetSession->Close();//关闭废弃的会话
//		this->ConnectFtp();//保持持续会话
//		pFtpConnection->SetCurrentDirectory(cstrFilePath);
//		this->UpdateDir();//更新目录列表
//	}  
//
//	fFinder.Close();
//}


void CShare_file::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
    DestroyWindow() ;
}


void CShare_file::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialog::PostNcDestroy();

	theApp.str_LoadPath = "" ;
	pInternetSession->Close();//结束会话
	m_listBox.ResetContent();

	delete theApp.share_file ;
	theApp.share_file = NULL ;
}
