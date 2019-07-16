
// MyChattingroom_client.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "MyChattingroom_clientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyChattingroom_clientApp

BEGIN_MESSAGE_MAP(CMyChattingroom_clientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMyChattingroom_clientApp 构造

CMyChattingroom_clientApp::CMyChattingroom_clientApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMyChattingroom_clientApp 对象

CMyChattingroom_clientApp theApp;


// CMyChattingroom_clientApp 初始化

BOOL CMyChattingroom_clientApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("Hello！"));

	srand((unsigned)time(0));
	mytcp = NULL;
	myudp = NULL;
	search = 0;     //服务器搜寻标识
	tag = 0;        //帐号和密码匹配标识
	nTime = 0 ;
	str_LoadPath = "" ;
	GetLocalIp(mylocalip);//获取本地IP
	int a=1;
	myudp = new CUdpSocket();//创建UDP套接字
	if(!myudp)
	{
		AfxMessageBox(_T("内存不足！"));
		exit(0);
	}
	if(!myudp->Create(1000,SOCK_DGRAM))
	{
		AfxMessageBox(_T("创建套接字失败！"));
		exit(0);
	}
	myudp->SetSockOpt(SO_BROADCAST,&a,sizeof(BOOL),SOL_SOCKET);//设置套接字选项，允许发送广播消息
	CString st("QQ客户端查找服务器");
	myudp->SendTo(st,2*st.GetLength(),1001,_T("255.255.255.255"));//发送广播
	if(login.DoModal()!=IDOK)
	{
		exit(0);
	}
	m_userid = login.myuserstr;

	CMyChattingroom_clientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		// “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//“取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

void CMyChattingroom_clientApp::GetLocalIp(CString &ip)
{
	char ipname[20] = {0}, **pptr;
	struct hostent *hptr;
	char str[32] = {0};
	int ret = gethostname(ipname,sizeof(ipname));//获取本地主机名
	hptr = gethostbyname(ipname);//通过域名改成IP
	switch(hptr->h_addrtype)
	{
	case AF_INET:
	case AF_INET6:
		pptr=hptr->h_addr_list;
		inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str));
		break;
	default:
		break;
	}
	ip = str;
}

