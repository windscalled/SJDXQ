// Mymusic.cpp : 实现文件
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Mymusic.h"
#include "afxdialogex.h"
#include <MMSystem.h>
#include <Digitalv.h>
#include <vector>
#include <functional>
#include <algorithm>

#pragma comment(lib, "Winmm.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND m_hWnd;
DWORD DeviceId;
MCI_OPEN_PARMS mciopenparms;
// CMymusic 对话框

IMPLEMENT_DYNAMIC(CMymusic, CDialogEx)

CMymusic::CMymusic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMymusic::IDD, pParent)
	, myint(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	myint = 0;
	theApp.music = this ;
}

CMymusic::~CMymusic()
{
}

void CMymusic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myint);
	DDX_Control(pDX, IDC_SLIDER1, myslider);
	DDX_Control(pDX, IDC_LIST1, m_music_listcontrol);
}


BEGIN_MESSAGE_MAP(CMymusic, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CMymusic::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON1, &CMymusic::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMymusic::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMymusic::OnBnClickedButton3)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(ID_BTN_OPEN_FILE, &CMymusic::OnBnClickedButton4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMymusic::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMymusic::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMymusic::OnNMRClickList1)
END_MESSAGE_MAP()


// CMymusic 消息处理程序

BOOL CMymusic::OnInitDialog()
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
	myslider.SetRange(0, 1000); //滑动条的移动范围 0~1000
	myslider.SetPos(500); //滑动条的指针处置位置为500,代表初试音量为500
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false); //文件没有读取时所有按钮不可选
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	

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

	//设置listcontrol控件
	CRect rect;  
	//获取编程语言列表视图控件的位置和大小
	m_music_listcontrol.GetClientRect(&rect); 
	//为列表视图控件添加全行选中和栅格风格
	m_music_listcontrol.SetExtendedStyle(m_music_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);	
	//为列表视图控件添加五列  
	m_music_listcontrol.InsertColumn(0, _T("名称"), LVCFMT_CENTER, rect.Width()*1/2, 1);
	m_music_listcontrol.InsertColumn(1, _T("名称"), LVCFMT_CENTER, rect.Width()*1/2, 1);
	m_music_listcontrol.InsertColumn(2, _T("本地路径"), LVCFMT_CENTER, rect.Width()*1/2, 1);
	m_music_listcontrol.DeleteColumn(0) ;

	m_music_listcontrol.ShowScrollBar(SB_VERT,TRUE) ;

	//设置行高
	CImageList m_l ;    
	m_l.Create(1,26,TRUE|ILC_COLOR32,1,0);
	m_music_listcontrol.SetImageList(&m_l,LVSIL_STATE) ;	

	m_music_listcontrol.SetBkColor(CLR_NONE) ;      //设置表格背景颜色	
	m_music_listcontrol.SetTextColor(RGB(0,0,0)) ;  //字体颜色
	m_music_listcontrol.SetTextBkColor(CLR_NONE) ;  //字体背景颜色

	HBITMAP hbit_button1, hbit_button2, hbit_button3, hbit_button4, hbit_button5;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/surf.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/play.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );	
	hbit_button4 = (HBITMAP)::LoadImage(NULL,"../../picture/stop.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );	
	hbit_button5 = (HBITMAP)::LoadImage(NULL,"../../picture/ting.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );	

	((CButton*)GetDlgItem(IDC_BUTTON4))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button3) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button4) ;
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hbit_button5) ;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


int alreadyadd = 0;
std::vector<CString> myvec;
void CMymusic::OnBnClickedButton4()  //浏览
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = TEXT("");
	strFilter = strFilter + TEXT("mp3文件(*.mp3)|*.mp3|") +
		TEXT("wma文件(*.wma)|*.wma|") + 
		TEXT("wav文件(*.wav)|*.wav|") + 
		TEXT("所有文件(*.*)|*.*|");
	CString filepath ;
	CString filename ;
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY, strFilter);//打开模态对话框
	
	if(dlg.DoModal() == IDOK)//成功
	{
		filepath = dlg.GetPathName();
		filename = dlg.GetFileName();
		SetDlgItemText(IDC_COMBO1,filename);		
	
		//文件读取成功时所有按钮变成可选
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	}
	else
	{
		return ;
	}

	//组合框
	TCHAR ws[256] ;
	CComboBox* pfix = (CComboBox*)GetDlgItem(IDC_COMBO1) ;
	pfix->GetWindowText(ws,256);
	TCHAR strCBText[256];
	int i ;
	for(i=0;i<50;i++)
	{
		pfix->GetLBText( i, strCBText);
		if(_tcscmp(ws,strCBText) == 0 )
		{
			alreadyadd = 1 ;
			return ;
		}
	}
	pfix->AddString(ws) ;
	myvec.push_back(filepath) ;  

	//listcontrol控件显示
	m_music_listcontrol.InsertItem(0,filename) ;
	m_music_listcontrol.SetItemText(0,1,filepath);

	
}

void musicplay()//播放音乐
{

	MCI_PLAY_PARMS mciplayparms ;
	mciplayparms.dwCallback = (DWORD)m_hWnd ;
	mciplayparms.dwFrom = 0 ; //音乐播放时从头开始
	mciSendCommand(DeviceId,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD)(LPVOID)&mciplayparms) ;

}

void FileLoad(HWND hWnd,CString filepath)
{
	m_hWnd = hWnd;
	mciSendCommand(DeviceId,MCI_CLOSE,0,0);//在加载文件前先清空上一次播放的设备
	mciopenparms.lpstrElementName = filepath ;//播放音乐文件路径传给设备

	DWORD myreturn ;
	if(myreturn =mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_WAIT,(DWORD)(LPVOID)&mciopenparms))
	{
		//打开音乐失败，将出错信息存放在buffer中，并给予警告提示
		char buffer[256] ;
		mciGetErrorString(myreturn,buffer,256);//获取错误码对应的错误信息
		MessageBox(hWnd,buffer,"错误警告！",MB_ICONHAND|MB_ICONERROR|MB_ICONSTOP) ;//弹出错误提示对话框
	}
	else
	{
		DeviceId = mciopenparms.wDeviceID;

	}

}

void CMymusic::OnBnClickedButton1() //音乐播放
{
	// TODO: 在此添加控件通知处理程序代码
	CString ws ;
	CComboBox* pfix = (CComboBox*)GetDlgItem(IDC_COMBO1) ;

	std::vector<CString>::iterator pmusic = myvec.end() ;
	std::vector<CString>::iterator pmusic2 = myvec.begin() ;

	int num ;
	num = -1 ;
	num = pfix->GetCurSel();
	if(num<0)//判断有无在下拉列表中选中某一栏
	{
		if(alreadyadd == 1)//判断歌曲是否已经添加
		{

			pfix->GetWindowText(ws);
			CString temper ;
			int i ;
			for(i=0;i<20;i++)
			{
				pfix->GetLBText( i, temper);
				if(_tcscmp(ws,temper) == 0 )//找到已添加歌曲的具体位置
				{

					CString road = *(pmusic2 + i) ;
					FileLoad(m_hWnd,road);
					alreadyadd = 0 ;
					musicplay();
					return ;
				}
			}

		}
		else
		{			
			CString road = *(pmusic - 1) ;
			FileLoad(m_hWnd,road);
		}

	}
	else
	{
		CString strCBText;
		pfix->GetLBText( num, strCBText);
		pfix->GetWindowText(ws);
		if(strCBText == ws)//判断此时添加歌名是否和当前选中歌名一致
		{
			CString road = *(pmusic2 + num) ;
			FileLoad(m_hWnd,road);
		}
		else
		{
			if(alreadyadd == 1)//判断歌曲是否已经添加
			{
				CString temper ;
				int i ;
				for(i=0;i<20;i++)
				{
					pfix->GetLBText( i, temper);
					if(_tcscmp(ws,temper) == 0 )//找到已添加歌曲的具体位置
					{
						CString road = *(pmusic2 + i) ;
						FileLoad(m_hWnd,road);
						alreadyadd = 0 ;
						musicplay();
						return ;
					}
				}

			}
			else
			{		
				CString road = *(pmusic - 1) ;
				FileLoad(m_hWnd,road);
			}

		}

	}

	musicplay();	
}

void mypause()//请求设备停止播放
{
	mciSendCommand(DeviceId,MCI_PAUSE,0,0) ;
}

void myresume()//请求设备恢复播放
{
	mciSendCommand(DeviceId,MCI_RESUME,0,0) ;
}

void CMymusic::OnBnClickedButton2()   //暂停
{
	// TODO: 在此添加控件通知处理程序代码
	CString mystate ;
	CString cs ;
	GetDlgItemText(IDC_BUTTON2,mystate) ;//获取按钮指示
	CString str1 = TEXT("暂停");
	CString str2 = TEXT("恢复");
	if(mystate == str1 )
	{
		mypause() ;
		SetDlgItemText(IDC_BUTTON2,str2) ;
	}
	else if(mystate == str2)
	{
		myresume() ;
		SetDlgItemText(IDC_BUTTON2,str1) ;
	}
}

void mystop()//请求设备停止播放，所有信息清除
{
	mciSendCommand(DeviceId,MCI_STOP,0,0) ;
	mciSendCommand(DeviceId,MCI_CLOSE,0,0) ;
}

void CMymusic::OnBnClickedButton3()   //停止
{
	// TODO: 在此添加控件通知处理程序代码
	mystop() ;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false) ;//播放和暂停不可选
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false) ;
}

DWORD setvolume(DWORD myvolume)//设置音量
{
	MCI_DGV_SETAUDIO_PARMS setvol ; //设置音量的参数结构体
	setvol.dwCallback = NULL ;
	setvol.dwItem = MCI_DGV_SETAUDIO_VOLUME ;//设置音量
	setvol.dwValue = myvolume ;//音量值
	mciSendCommand(DeviceId,MCI_SETAUDIO,MCI_DGV_SETAUDIO_ITEM|MCI_DGV_SETAUDIO_VALUE,(DWORD)(LPVOID)&setvol) ;

	return 0 ;
}



void CMymusic::OnPaint()
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


void CMymusic::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(true) ;
	myint = myslider.GetPos() /10 ;
	//myint = m_nSlider ;
	setvolume(myslider.GetPos()) ;
	UpdateData(false) ;

	*pResult = 0;
}


HBRUSH CMymusic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
		return hbr;;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CMymusic::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		//CAboutDlg dlgAbout;
		//dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMymusic::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CMymusic::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString strLoad ;  
	CString filename ;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择
	{
		filename = m_music_listcontrol.GetItemText(pNMListView->iItem, 0);
		strLoad = m_music_listcontrol.GetItemText(pNMListView->iItem, 1); 
		SetDlgItemText(IDC_COMBO1,filename);
		FileLoad(m_hWnd,strLoad);
		musicplay();
	}		
	else
	{
		AfxMessageBox("双击播放，请添加音乐文件！");
		return;
	}
}


void CMymusic::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if( index == -1 )
		return;
	HMENU hMenu = ::CreatePopupMenu();
	AppendMenu( hMenu, MF_STRING, 10001, _T("删除歌曲") );
	CPoint pt;
	GetCursorPos(&pt); //获得当前鼠标位置
	UINT Cmd = (UINT)::TrackPopupMenu( hMenu, TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL );//弹出菜单

	switch( Cmd )//响应点击的菜单
	{
	case 10001:
		{
			POSITION p=m_music_listcontrol.GetFirstSelectedItemPosition();    //获取首选中行位置
			while (p) 
			{
				int  nSelected=m_music_listcontrol.GetNextSelectedItem(p); //获取选中行的索引			
				CString strDeleteNum = m_music_listcontrol.GetItemText(nSelected,0) ;
				m_music_listcontrol.DeleteItem(nSelected); //根据索引删除
				p = m_music_listcontrol.GetFirstSelectedItemPosition();  
			}
			break;
		}

	}
}
