
// MyChattingroom_clientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h" 
#include "MyChattingroom_clientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyChattingroom_clientDlg 对话框




CMyChattingroom_clientDlg::CMyChattingroom_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyChattingroom_clientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.active = this;
	theApp.myudp->Close();//关闭UDP套接字
	theApp.myudp = NULL;
}

void CMyChattingroom_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mylistcontrol);
}

BEGIN_MESSAGE_MAP(CMyChattingroom_clientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMyChattingroom_clientDlg::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyChattingroom_clientDlg::OnBnClickedButton1)
	ON_WM_CTLCOLOR()

	ON_MESSAGE(MY_NOTIFYICON, &CMyChattingroom_clientDlg::OnMyIconNotify)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyChattingroom_clientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_Music, &CMyChattingroom_clientDlg::OnBnClickedMusic)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CMyChattingroom_clientDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyChattingroom_clientDlg::OnBnClickedButton3)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMyChattingroom_clientDlg 消息处理程序

BOOL CMyChattingroom_clientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString name = "MCQ:"+theApp.m_username+"("+theApp.m_userid+")" ;   //托盘提示
	HICON hicon =(HICON)LoadImage(NULL,"../MyChattingroom_client/res/MyChattingroom_client.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	TaskBarAddIcon(GetSafeHwnd(),120,hicon,name);  //添加任务栏图标

	CRect rect;
	// 获取编程语言列表视图控件的位置和大小
	mylistcontrol.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格
	mylistcontrol.SetExtendedStyle(mylistcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加两列
	mylistcontrol.InsertColumn(0, _T("在线用户"), LVCFMT_CENTER, rect.Width()*3/10, 1);
	mylistcontrol.InsertColumn(1, _T("在线用户"), LVCFMT_CENTER, rect.Width()*3/10, 1);
	mylistcontrol.InsertColumn(2, _T("昵称"), LVCFMT_CENTER, rect.Width()*3/10, 1);
	mylistcontrol.InsertColumn(3, _T("用户IP"), LVCFMT_CENTER, rect.Width()*4/10, 1);
	mylistcontrol.DeleteColumn(0);

	//设置行高
	CImageList m_l ;    
	m_l.Create(1,23,TRUE|ILC_COLOR32,1,0);
	mylistcontrol.SetImageList(&m_l,LVSIL_STATE) ;	

	mylistcontrol.SetBkColor(CLR_NONE) ;       //设置表格背景颜色	
	mylistcontrol.SetTextColor(RGB(0,0,0)) ;   //字体颜色
	mylistcontrol.SetTextBkColor(CLR_NONE) ;   //字体背景颜色

	HBITMAP hbit_button1, hbit_button2,hbit_button3,hbit_button4,hbit_button5;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/chat.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/music.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/exit.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button4 = (HBITMAP)::LoadImage(NULL,"../../picture/up_file.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button5 = (HBITMAP)::LoadImage(NULL,"../../picture/baidu.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_Music))->SetBitmap(hbit_button2) ;
	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button3) ;
	((CButton*)GetDlgItem(IDC_BUTTON4))->SetBitmap(hbit_button4) ;
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hbit_button5) ;

	m_caption = (CString)"(" + theApp.m_userid + (CString)") " + theApp.m_username + (CString)"的MCQ";
	SetWindowText(m_caption);


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

	CTime time=time.GetCurrentTime();
	CString str=time.Format("%Y-%m-%d %X");
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);

	CRect dlgRect;
	GetWindowRect(dlgRect);
	CRect desktopRect;
	//将窗口开始大小设为0
	GetDesktopWindow()->GetWindowRect(desktopRect);
	MoveWindow((desktopRect.Width() - dlgRect.Width()) / 2,
		(desktopRect.Height() - dlgRect.Height()) / 2,
		0,
		0 );

	//初始化变化大小
	m_nWidth=dlgRect.Width();
	m_nHeight=dlgRect.Height();
	m_nDx=4;
	m_nDy=4;
	this->SetTimer(1,10,NULL);
	this->SetTimer(2,1000,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyChattingroom_clientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID==SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyChattingroom_clientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyChattingroom_clientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyChattingroom_clientDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)//双击列表视图控件事件响应程序
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strLangID;    
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择
	{
		strLangID = mylistcontrol.GetItemText(pNMListView->iItem, 0); //获取聊天对象的id
		
	}		
	else
	{
		return;
	}
		
	map<CString,CChat *>::iterator it = theApp.m_usermessage.find(strLangID); //匹配聊天对话框的信息
	if(it == theApp.m_usermessage.end()) //未找到则创建
	{
		CChat *c;
		c = new CChat;
		c->mytoid = strLangID;
		c->othername = theApp.m_client.find(strLangID)->second;//获取聊天对象的昵称
		c->Create(IDD_CHAT,this);
		c->ShowWindow(SW_SHOW);
		theApp.m_usermessage.insert(make_pair(strLangID,c));
	}
	else
	{
		return;
	}
		
}



void CMyChattingroom_clientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	TaskBarDeleteIcon( GetSafeHwnd(),120);
	exit(0) ;
}

void CMyChattingroom_clientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CDialogEx::OnClose();
	return;
}


HBRUSH CMyChattingroom_clientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd == this)
	{
		return m_Br2;

	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); //设置背景为透明
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMyChattingroom_clientDlg::OnBnClickedButton2()  //创建群聊对话框
{
	// TODO: 在此添加控件通知处理程序代码
	CString group = (CString)"10000";
	CString groupname =(CString) "我们的讨论群" ;
	map<CString,CChat *>::iterator it = theApp.m_usermessage.find(group); //匹配聊天对话框的信息
	if(it == theApp.m_usermessage.end()) //未找到则创建
	{
		CChat *c;
		c = new CChat;
		c->mytoid = group;
		c->othername = groupname;//获取聊天对象的昵称
		c->Create(IDD_CHAT,this);
		c->ShowWindow(SW_SHOW);
		c->GetDlgItem(IDC_file)->EnableWindow(FALSE);
		theApp.m_usermessage.insert(make_pair(group,c));
	}
	else
	{
		return;
	}

}


void CMyChattingroom_clientDlg::OnBnClickedMusic()
{
	// TODO: 在此添加控件通知处理程序代码
	if(music.m_hWnd == 0)//判断有没有创建过
	{

		music.Create(IDD_DIALOG6,this);

	}
	music.ShowWindow(SW_SHOW);
}


void CMyChattingroom_clientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case 1 :
		{
			CRect dlgRect;
			GetWindowRect(dlgRect);

			//获得桌面的大小
			CRect desktopRect;
			GetDesktopWindow()->GetWindowRect(desktopRect);

			//如果是窗口弹出过程，则逐渐增大窗口
			if(nIDEvent == 1)
			{
				MoveWindow(
					(-m_nDx+desktopRect.Width() - dlgRect.Width()) /2,
					(-m_nDy+desktopRect.Height() - dlgRect.Height()) /2, 
					m_nWidth,
					+m_nDy+dlgRect.Height() );

				//不要超过窗口预设的宽度
				if(dlgRect.Width() >=m_nWidth) 
				{
					m_nDx=0; 
				}

				//不要超过窗口预设的高度
				if(dlgRect.Height() >=m_nHeight)
				{
					m_nDy=0;
				}		

				//停止变化，关闭定时器1
				if((dlgRect.Width() >=m_nWidth) && (dlgRect.Height() >=m_nHeight))
				{
					KillTimer(1); 
				}			
			}

			break;
		}

	case 2:
		{
			CTime time=time.GetCurrentTime();
			CString str=time.Format("%Y-%m-%d %X");
			GetDlgItem(IDC_EDIT1)->SetWindowText(str);
			break;
		}
		
	}
	
}



void CMyChattingroom_clientDlg::OnBnClickedButton4()  //资源共享
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.share_file = new CShare_file() ;
	theApp.share_file->Create(IDD_DIALOG8,NULL) ;
	theApp.share_file->ShowWindow(SW_SHOW);
}


void CMyChattingroom_clientDlg::OnBnClickedButton3()  //百度
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","http://www.baidu.com",NULL,NULL,SW_SHOWNORMAL) ;
}


//添加托盘图标函数
BOOL CMyChattingroom_clientDlg::TaskBarAddIcon( HWND hwnd,UINT uID,HICON hicon,CString strTip )
{
	NOTIFYICONDATA d;//托盘图标结构体
	d.cbSize = sizeof(NOTIFYICONDATA);//托盘图标结构体的大小
	d.hWnd = hwnd;//托盘图标所属的窗体
	d.uID = uID;//托盘图标ID
	d.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;//托盘图标结构体的属性
	d.uCallbackMessage = MY_NOTIFYICON;//设置托盘图标的单击消息
	d.hIcon =  hicon;
	lstrcpy	(d.szTip,strTip);//设置托盘图标的提示

	return Shell_NotifyIcon(NIM_ADD,&d);//添加托盘图标

}

LRESULT CMyChattingroom_clientDlg::OnMyIconNotify( WPARAM wparam,LPARAM lparam )
{
	UINT uMouseMsg = LOWORD(lparam);
	switch(uMouseMsg)
	{
	case WM_LBUTTONDOWN:
		{
			ShowWindow(IsWindowVisible()?SW_HIDE:SW_SHOWNORMAL);
		}
		break;
	}

	return 0;
}


//删除托盘图标函数
BOOL CMyChattingroom_clientDlg::TaskBarDeleteIcon( HWND hwnd,UINT uID )
{
	NOTIFYICONDATA d;
	d.cbSize = sizeof(NOTIFYICONDATA);
	d.hWnd = hwnd;
	d.uID = uID;
	return Shell_NotifyIcon(NIM_DELETE,&d);//删除托盘图标
}


void CMyChattingroom_clientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	TaskBarDeleteIcon( GetSafeHwnd(),120);
}
