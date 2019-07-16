
// MyChattingroom_serverDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_server.h"
#include "MyChattingroom_serverDlg.h"
#include "afxdialogex.h"
#include <mysql.h>

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


// CMyChattingroom_serverDlg 对话框




CMyChattingroom_serverDlg::CMyChattingroom_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyChattingroom_serverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	theApp.active = this;
	server_start = false;//服务器启动标识初始化为0
	m_udpsocket = NULL;
	m_tcpsocket = NULL;
	tcp_start = 0;        //tcp启动标识
	srand((unsigned)time(0));
}

void CMyChattingroom_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, my_listcontrol);
	DDX_Control(pDX, IDC_start, button1ctr);
	DDX_Control(pDX, IDC_exit, button2ctr);
	DDX_Control(pDX, IDC_LIST2, m_fileListcontrol);
}

BEGIN_MESSAGE_MAP(CMyChattingroom_serverDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_start, &CMyChattingroom_serverDlg::OnBnClickedstart)
	ON_BN_CLICKED(IDC_exit, &CMyChattingroom_serverDlg::OnBnClickedexit)	
	ON_MESSAGE(WM_MSG,OnMsg)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMyChattingroom_serverDlg 消息处理程序

BOOL CMyChattingroom_serverDlg::OnInitDialog()
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
	//创建一个mysql表格
	 theApp.my_mysql();
	 mysql_query(theApp.mysql,"create table information(nicname varchar(20),qqnum varchar(6),password varchar(20),question varchar(200),answer varchar(200),primary key(qqnum));");
	 mysql_query(theApp.mysql,"create table file_infor(nicname varchar(20),state varchar(200),time datetime);");
	
	CRect rect;
    my_listcontrol.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格
	my_listcontrol.SetExtendedStyle(my_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加两列
	my_listcontrol.InsertColumn(0, _T("用户账号"), LVCFMT_CENTER, rect.Width()/2, 1);
	my_listcontrol.InsertColumn(1, _T("用户账号"), LVCFMT_CENTER, rect.Width()/2, 1);
	my_listcontrol.InsertColumn(2, _T("用户IP"), LVCFMT_CENTER, rect.Width()/2, 1);
	my_listcontrol.DeleteColumn(0);


	CRect rect2;
	m_fileListcontrol.GetClientRect(&rect2);
	// 为列表视图控件添加全行选中和栅格风格
	m_fileListcontrol.SetExtendedStyle(m_fileListcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加两列
	m_fileListcontrol.InsertColumn(0, _T("用户名"), LVCFMT_CENTER, rect2.Width()/6*1, 1);
	m_fileListcontrol.InsertColumn(1, _T("用户名"), LVCFMT_CENTER, rect2.Width()/6*1, 1);
	m_fileListcontrol.InsertColumn(2, _T("资源动态"), LVCFMT_CENTER, rect2.Width()/6*3, 1);
	m_fileListcontrol.InsertColumn(3, _T("操作时间"), LVCFMT_CENTER, rect2.Width()/6*2, 1);
	m_fileListcontrol.DeleteColumn(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyChattingroom_serverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyChattingroom_serverDlg::OnPaint()
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
HCURSOR CMyChattingroom_serverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyChattingroom_serverDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(server_start)
	{
		return;
	}
	CDialogEx::OnClose();
}




void CMyChattingroom_serverDlg::OnBnClickedstart()   //启动/断开服务器
{
	// TODO: 在此添加控件通知处理程序代码
	if(!server_start)//服务器未启动
	{
		if(m_udpsocket == NULL)
		{
			m_udpsocket = new CUdpSocket();
			m_udpsocket->Create(1001,SOCK_DGRAM);
		}
		button1ctr.SetWindowText(_T("断开"));
		server_start= true;
		button2ctr.EnableWindow(false);
	}
	else//服务器已启动
	{
		if(m_udpsocket != NULL)
		{
			m_udpsocket->Close();
			delete m_udpsocket;
			m_udpsocket = NULL;
		}
		if(m_tcpsocket != NULL)
		{
			m_tcpsocket->Close();
			delete m_tcpsocket;
			m_tcpsocket = NULL;
		}
		button1ctr.SetWindowText(_T("启动"));
		server_start = false;
		tcp_start = 0;
		my_listcontrol.DeleteAllItems();
		button2ctr.EnableWindow(true);    //使能退出按钮
	}
}


void CMyChattingroom_serverDlg::OnBnClickedexit()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}

LRESULT CMyChattingroom_serverDlg::OnMsg(WPARAM wParam,LPARAM lParam)//自定义消息响应
{
	map<CString,CString>::iterator it;
	int i;
	switch(theApp.head.type)
	{
	case 0://启动TCP
		{
			if(tcp_start == 0)//tcp启动标识，每次启动服务器，tcp套接字只create一次
			{
				m_tcpsocket = new CTcpSocket();
				if(!m_tcpsocket->Create(1003))
				{
					MessageBox(_T("创建套接字错误！"));
					m_tcpsocket->Close();
				}
				else if(!m_tcpsocket->Listen())
				{
					MessageBox(_T("监听失败！"));
					m_tcpsocket->Close();
				}
				tcp_start = 1;
			}
			break;
		}	
	case 1:  //更新在线用户信息
		{
			// 在列表视图控件中插入列表项，并设置列表子项文本
			my_listcontrol.DeleteAllItems();
			for(it = theApp.m_clientMessage.begin(),i = 0;it != theApp.m_clientMessage.end();it++,i++)
			{
				my_listcontrol.InsertItem(i, it->first);
				my_listcontrol.SetItemText(i, 1, it->second);
			}
			break;
		}		
	case 2:  //资源动态
		{
			m_fileListcontrol.InsertItem(0,theApp.file_nicname);
			m_fileListcontrol.SetItemText(0,1,theApp.file_state);
			m_fileListcontrol.SetItemText(0,2,theApp.file_time);

			int result;
			// 没有返回结果集的操作
			theApp.my_mysql();
			char buff[500];
			sprintf(buff,"insert into file_infor values('%s','%s','%s');",theApp.file_nicname,theApp.file_state,theApp.file_time);

			result = mysql_query(theApp.mysql,buff);
			if (result != 0)
			{
				AfxMessageBox(_T("上传资源动态失败，请检查！"));
				AfxMessageBox((CString)mysql_error(theApp.mysql));
			}
			break;
		}
	default:
		break;
	}
	return TRUE;
}
