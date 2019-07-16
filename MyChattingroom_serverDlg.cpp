
// MyChattingroom_serverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_server.h"
#include "MyChattingroom_serverDlg.h"
#include "afxdialogex.h"
#include <mysql.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMyChattingroom_serverDlg �Ի���




CMyChattingroom_serverDlg::CMyChattingroom_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyChattingroom_serverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	theApp.active = this;
	server_start = false;//������������ʶ��ʼ��Ϊ0
	m_udpsocket = NULL;
	m_tcpsocket = NULL;
	tcp_start = 0;        //tcp������ʶ
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


// CMyChattingroom_serverDlg ��Ϣ�������

BOOL CMyChattingroom_serverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����һ��mysql���
	 theApp.my_mysql();
	 mysql_query(theApp.mysql,"create table information(nicname varchar(20),qqnum varchar(6),password varchar(20),question varchar(200),answer varchar(200),primary key(qqnum));");
	 mysql_query(theApp.mysql,"create table file_infor(nicname varchar(20),state varchar(200),time datetime);");
	
	CRect rect;
    my_listcontrol.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	my_listcontrol.SetExtendedStyle(my_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// Ϊ�б���ͼ�ؼ��������
	my_listcontrol.InsertColumn(0, _T("�û��˺�"), LVCFMT_CENTER, rect.Width()/2, 1);
	my_listcontrol.InsertColumn(1, _T("�û��˺�"), LVCFMT_CENTER, rect.Width()/2, 1);
	my_listcontrol.InsertColumn(2, _T("�û�IP"), LVCFMT_CENTER, rect.Width()/2, 1);
	my_listcontrol.DeleteColumn(0);


	CRect rect2;
	m_fileListcontrol.GetClientRect(&rect2);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	m_fileListcontrol.SetExtendedStyle(m_fileListcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// Ϊ�б���ͼ�ؼ��������
	m_fileListcontrol.InsertColumn(0, _T("�û���"), LVCFMT_CENTER, rect2.Width()/6*1, 1);
	m_fileListcontrol.InsertColumn(1, _T("�û���"), LVCFMT_CENTER, rect2.Width()/6*1, 1);
	m_fileListcontrol.InsertColumn(2, _T("��Դ��̬"), LVCFMT_CENTER, rect2.Width()/6*3, 1);
	m_fileListcontrol.InsertColumn(3, _T("����ʱ��"), LVCFMT_CENTER, rect2.Width()/6*2, 1);
	m_fileListcontrol.DeleteColumn(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyChattingroom_serverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyChattingroom_serverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyChattingroom_serverDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(server_start)
	{
		return;
	}
	CDialogEx::OnClose();
}




void CMyChattingroom_serverDlg::OnBnClickedstart()   //����/�Ͽ�������
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!server_start)//������δ����
	{
		if(m_udpsocket == NULL)
		{
			m_udpsocket = new CUdpSocket();
			m_udpsocket->Create(1001,SOCK_DGRAM);
		}
		button1ctr.SetWindowText(_T("�Ͽ�"));
		server_start= true;
		button2ctr.EnableWindow(false);
	}
	else//������������
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
		button1ctr.SetWindowText(_T("����"));
		server_start = false;
		tcp_start = 0;
		my_listcontrol.DeleteAllItems();
		button2ctr.EnableWindow(true);    //ʹ���˳���ť
	}
}


void CMyChattingroom_serverDlg::OnBnClickedexit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	exit(0);
}

LRESULT CMyChattingroom_serverDlg::OnMsg(WPARAM wParam,LPARAM lParam)//�Զ�����Ϣ��Ӧ
{
	map<CString,CString>::iterator it;
	int i;
	switch(theApp.head.type)
	{
	case 0://����TCP
		{
			if(tcp_start == 0)//tcp������ʶ��ÿ��������������tcp�׽���ֻcreateһ��
			{
				m_tcpsocket = new CTcpSocket();
				if(!m_tcpsocket->Create(1003))
				{
					MessageBox(_T("�����׽��ִ���"));
					m_tcpsocket->Close();
				}
				else if(!m_tcpsocket->Listen())
				{
					MessageBox(_T("����ʧ�ܣ�"));
					m_tcpsocket->Close();
				}
				tcp_start = 1;
			}
			break;
		}	
	case 1:  //���������û���Ϣ
		{
			// ���б���ͼ�ؼ��в����б���������б������ı�
			my_listcontrol.DeleteAllItems();
			for(it = theApp.m_clientMessage.begin(),i = 0;it != theApp.m_clientMessage.end();it++,i++)
			{
				my_listcontrol.InsertItem(i, it->first);
				my_listcontrol.SetItemText(i, 1, it->second);
			}
			break;
		}		
	case 2:  //��Դ��̬
		{
			m_fileListcontrol.InsertItem(0,theApp.file_nicname);
			m_fileListcontrol.SetItemText(0,1,theApp.file_state);
			m_fileListcontrol.SetItemText(0,2,theApp.file_time);

			int result;
			// û�з��ؽ�����Ĳ���
			theApp.my_mysql();
			char buff[500];
			sprintf(buff,"insert into file_infor values('%s','%s','%s');",theApp.file_nicname,theApp.file_state,theApp.file_time);

			result = mysql_query(theApp.mysql,buff);
			if (result != 0)
			{
				AfxMessageBox(_T("�ϴ���Դ��̬ʧ�ܣ����飡"));
				AfxMessageBox((CString)mysql_error(theApp.mysql));
			}
			break;
		}
	default:
		break;
	}
	return TRUE;
}
