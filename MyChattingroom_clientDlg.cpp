
// MyChattingroom_clientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h" 
#include "MyChattingroom_clientDlg.h"
#include "afxdialogex.h"

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


// CMyChattingroom_clientDlg �Ի���




CMyChattingroom_clientDlg::CMyChattingroom_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyChattingroom_clientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.active = this;
	theApp.myudp->Close();//�ر�UDP�׽���
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


// CMyChattingroom_clientDlg ��Ϣ�������

BOOL CMyChattingroom_clientDlg::OnInitDialog()
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
	CString name = "MCQ:"+theApp.m_username+"("+theApp.m_userid+")" ;   //������ʾ
	HICON hicon =(HICON)LoadImage(NULL,"../MyChattingroom_client/res/MyChattingroom_client.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
	TaskBarAddIcon(GetSafeHwnd(),120,hicon,name);  //���������ͼ��

	CRect rect;
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	mylistcontrol.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	mylistcontrol.SetExtendedStyle(mylistcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// Ϊ�б���ͼ�ؼ��������
	mylistcontrol.InsertColumn(0, _T("�����û�"), LVCFMT_CENTER, rect.Width()*3/10, 1);
	mylistcontrol.InsertColumn(1, _T("�����û�"), LVCFMT_CENTER, rect.Width()*3/10, 1);
	mylistcontrol.InsertColumn(2, _T("�ǳ�"), LVCFMT_CENTER, rect.Width()*3/10, 1);
	mylistcontrol.InsertColumn(3, _T("�û�IP"), LVCFMT_CENTER, rect.Width()*4/10, 1);
	mylistcontrol.DeleteColumn(0);

	//�����и�
	CImageList m_l ;    
	m_l.Create(1,23,TRUE|ILC_COLOR32,1,0);
	mylistcontrol.SetImageList(&m_l,LVSIL_STATE) ;	

	mylistcontrol.SetBkColor(CLR_NONE) ;       //���ñ�񱳾���ɫ	
	mylistcontrol.SetTextColor(RGB(0,0,0)) ;   //������ɫ
	mylistcontrol.SetTextBkColor(CLR_NONE) ;   //���屳����ɫ

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

	m_caption = (CString)"(" + theApp.m_userid + (CString)") " + theApp.m_username + (CString)"��MCQ";
	SetWindowText(m_caption);


	CString strPath = TEXT("../../picture/4.bmp");
	if(m_image1.Load((LPCTSTR)strPath) != S_OK) //����ͼƬ·������ͼƬ 
	{
		AfxMessageBox(TEXT("Load Error!"));
		return FALSE;
	}
	HBITMAP hBit = m_image1;
	CBitmap cBit;
	m_Br2.DeleteObject() ; //������ʼ��
	CBitmap* pBit = cBit.FromHandle(hBit);
	m_Br2.CreatePatternBrush(pBit);
	cBit.DeleteObject();

	CTime time=time.GetCurrentTime();
	CString str=time.Format("%Y-%m-%d %X");
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);

	CRect dlgRect;
	GetWindowRect(dlgRect);
	CRect desktopRect;
	//�����ڿ�ʼ��С��Ϊ0
	GetDesktopWindow()->GetWindowRect(desktopRect);
	MoveWindow((desktopRect.Width() - dlgRect.Width()) / 2,
		(desktopRect.Height() - dlgRect.Height()) / 2,
		0,
		0 );

	//��ʼ���仯��С
	m_nWidth=dlgRect.Width();
	m_nHeight=dlgRect.Height();
	m_nDx=4;
	m_nDy=4;
	this->SetTimer(1,10,NULL);
	this->SetTimer(2,1000,NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyChattingroom_clientDlg::OnPaint()
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
HCURSOR CMyChattingroom_clientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMyChattingroom_clientDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)//˫���б���ͼ�ؼ��¼���Ӧ����
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString strLangID;    
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��
	{
		strLangID = mylistcontrol.GetItemText(pNMListView->iItem, 0); //��ȡ��������id
		
	}		
	else
	{
		return;
	}
		
	map<CString,CChat *>::iterator it = theApp.m_usermessage.find(strLangID); //ƥ������Ի������Ϣ
	if(it == theApp.m_usermessage.end()) //δ�ҵ��򴴽�
	{
		CChat *c;
		c = new CChat;
		c->mytoid = strLangID;
		c->othername = theApp.m_client.find(strLangID)->second;//��ȡ���������ǳ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TaskBarDeleteIcon( GetSafeHwnd(),120);
	exit(0) ;
}

void CMyChattingroom_clientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialogEx::OnClose();
	return;
}


HBRUSH CMyChattingroom_clientDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;

	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CMyChattingroom_clientDlg::OnBnClickedButton2()  //����Ⱥ�ĶԻ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString group = (CString)"10000";
	CString groupname =(CString) "���ǵ�����Ⱥ" ;
	map<CString,CChat *>::iterator it = theApp.m_usermessage.find(group); //ƥ������Ի������Ϣ
	if(it == theApp.m_usermessage.end()) //δ�ҵ��򴴽�
	{
		CChat *c;
		c = new CChat;
		c->mytoid = group;
		c->othername = groupname;//��ȡ���������ǳ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(music.m_hWnd == 0)//�ж���û�д�����
	{

		music.Create(IDD_DIALOG6,this);

	}
	music.ShowWindow(SW_SHOW);
}


void CMyChattingroom_clientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case 1 :
		{
			CRect dlgRect;
			GetWindowRect(dlgRect);

			//�������Ĵ�С
			CRect desktopRect;
			GetDesktopWindow()->GetWindowRect(desktopRect);

			//����Ǵ��ڵ������̣��������󴰿�
			if(nIDEvent == 1)
			{
				MoveWindow(
					(-m_nDx+desktopRect.Width() - dlgRect.Width()) /2,
					(-m_nDy+desktopRect.Height() - dlgRect.Height()) /2, 
					m_nWidth,
					+m_nDy+dlgRect.Height() );

				//��Ҫ��������Ԥ��Ŀ��
				if(dlgRect.Width() >=m_nWidth) 
				{
					m_nDx=0; 
				}

				//��Ҫ��������Ԥ��ĸ߶�
				if(dlgRect.Height() >=m_nHeight)
				{
					m_nDy=0;
				}		

				//ֹͣ�仯���رն�ʱ��1
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



void CMyChattingroom_clientDlg::OnBnClickedButton4()  //��Դ����
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	theApp.share_file = new CShare_file() ;
	theApp.share_file->Create(IDD_DIALOG8,NULL) ;
	theApp.share_file->ShowWindow(SW_SHOW);
}


void CMyChattingroom_clientDlg::OnBnClickedButton3()  //�ٶ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL,"open","http://www.baidu.com",NULL,NULL,SW_SHOWNORMAL) ;
}


//�������ͼ�꺯��
BOOL CMyChattingroom_clientDlg::TaskBarAddIcon( HWND hwnd,UINT uID,HICON hicon,CString strTip )
{
	NOTIFYICONDATA d;//����ͼ��ṹ��
	d.cbSize = sizeof(NOTIFYICONDATA);//����ͼ��ṹ��Ĵ�С
	d.hWnd = hwnd;//����ͼ�������Ĵ���
	d.uID = uID;//����ͼ��ID
	d.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;//����ͼ��ṹ�������
	d.uCallbackMessage = MY_NOTIFYICON;//��������ͼ��ĵ�����Ϣ
	d.hIcon =  hicon;
	lstrcpy	(d.szTip,strTip);//��������ͼ�����ʾ

	return Shell_NotifyIcon(NIM_ADD,&d);//�������ͼ��

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


//ɾ������ͼ�꺯��
BOOL CMyChattingroom_clientDlg::TaskBarDeleteIcon( HWND hwnd,UINT uID )
{
	NOTIFYICONDATA d;
	d.cbSize = sizeof(NOTIFYICONDATA);
	d.hWnd = hwnd;
	d.uID = uID;
	return Shell_NotifyIcon(NIM_DELETE,&d);//ɾ������ͼ��
}


void CMyChattingroom_clientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	TaskBarDeleteIcon( GetSafeHwnd(),120);
}
