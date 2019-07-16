
// MyChattingroom_client.cpp : ����Ӧ�ó��������Ϊ��
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


// CMyChattingroom_clientApp ����

CMyChattingroom_clientApp::CMyChattingroom_clientApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMyChattingroom_clientApp ����

CMyChattingroom_clientApp theApp;


// CMyChattingroom_clientApp ��ʼ��

BOOL CMyChattingroom_clientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Hello��"));

	srand((unsigned)time(0));
	mytcp = NULL;
	myudp = NULL;
	search = 0;     //��������Ѱ��ʶ
	tag = 0;        //�ʺź�����ƥ���ʶ
	nTime = 0 ;
	str_LoadPath = "" ;
	GetLocalIp(mylocalip);//��ȡ����IP
	int a=1;
	myudp = new CUdpSocket();//����UDP�׽���
	if(!myudp)
	{
		AfxMessageBox(_T("�ڴ治�㣡"));
		exit(0);
	}
	if(!myudp->Create(1000,SOCK_DGRAM))
	{
		AfxMessageBox(_T("�����׽���ʧ�ܣ�"));
		exit(0);
	}
	myudp->SetSockOpt(SO_BROADCAST,&a,sizeof(BOOL),SOL_SOCKET);//�����׽���ѡ������͹㲥��Ϣ
	CString st("QQ�ͻ��˲��ҷ�����");
	myudp->SendTo(st,2*st.GetLength(),1001,_T("255.255.255.255"));//���͹㲥
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
		// TODO: �ڴ˷��ô����ʱ��
		// ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

void CMyChattingroom_clientApp::GetLocalIp(CString &ip)
{
	char ipname[20] = {0}, **pptr;
	struct hostent *hptr;
	char str[32] = {0};
	int ret = gethostname(ipname,sizeof(ipname));//��ȡ����������
	hptr = gethostbyname(ipname);//ͨ�������ĳ�IP
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

