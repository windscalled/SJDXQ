// Mymusic.cpp : ʵ���ļ�
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
// CMymusic �Ի���

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


// CMymusic ��Ϣ�������

BOOL CMymusic::OnInitDialog()
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
	myslider.SetRange(0, 1000); //���������ƶ���Χ 0~1000
	myslider.SetPos(500); //��������ָ�봦��λ��Ϊ500,�����������Ϊ500
	GetDlgItem(IDC_BUTTON3)->EnableWindow(false); //�ļ�û�ж�ȡʱ���а�ť����ѡ
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	

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

	//����listcontrol�ؼ�
	CRect rect;  
	//��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	m_music_listcontrol.GetClientRect(&rect); 
	//Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	m_music_listcontrol.SetExtendedStyle(m_music_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);	
	//Ϊ�б���ͼ�ؼ��������  
	m_music_listcontrol.InsertColumn(0, _T("����"), LVCFMT_CENTER, rect.Width()*1/2, 1);
	m_music_listcontrol.InsertColumn(1, _T("����"), LVCFMT_CENTER, rect.Width()*1/2, 1);
	m_music_listcontrol.InsertColumn(2, _T("����·��"), LVCFMT_CENTER, rect.Width()*1/2, 1);
	m_music_listcontrol.DeleteColumn(0) ;

	m_music_listcontrol.ShowScrollBar(SB_VERT,TRUE) ;

	//�����и�
	CImageList m_l ;    
	m_l.Create(1,26,TRUE|ILC_COLOR32,1,0);
	m_music_listcontrol.SetImageList(&m_l,LVSIL_STATE) ;	

	m_music_listcontrol.SetBkColor(CLR_NONE) ;      //���ñ�񱳾���ɫ	
	m_music_listcontrol.SetTextColor(RGB(0,0,0)) ;  //������ɫ
	m_music_listcontrol.SetTextBkColor(CLR_NONE) ;  //���屳����ɫ

	HBITMAP hbit_button1, hbit_button2, hbit_button3, hbit_button4, hbit_button5;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/surf.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/play.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );	
	hbit_button4 = (HBITMAP)::LoadImage(NULL,"../../picture/stop.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );	
	hbit_button5 = (HBITMAP)::LoadImage(NULL,"../../picture/ting.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );	

	((CButton*)GetDlgItem(IDC_BUTTON4))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button3) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button4) ;
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hbit_button5) ;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


int alreadyadd = 0;
std::vector<CString> myvec;
void CMymusic::OnBnClickedButton4()  //���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilter = TEXT("");
	strFilter = strFilter + TEXT("mp3�ļ�(*.mp3)|*.mp3|") +
		TEXT("wma�ļ�(*.wma)|*.wma|") + 
		TEXT("wav�ļ�(*.wav)|*.wav|") + 
		TEXT("�����ļ�(*.*)|*.*|");
	CString filepath ;
	CString filename ;
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY, strFilter);//��ģ̬�Ի���
	
	if(dlg.DoModal() == IDOK)//�ɹ�
	{
		filepath = dlg.GetPathName();
		filename = dlg.GetFileName();
		SetDlgItemText(IDC_COMBO1,filename);		
	
		//�ļ���ȡ�ɹ�ʱ���а�ť��ɿ�ѡ
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	}
	else
	{
		return ;
	}

	//��Ͽ�
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

	//listcontrol�ؼ���ʾ
	m_music_listcontrol.InsertItem(0,filename) ;
	m_music_listcontrol.SetItemText(0,1,filepath);

	
}

void musicplay()//��������
{

	MCI_PLAY_PARMS mciplayparms ;
	mciplayparms.dwCallback = (DWORD)m_hWnd ;
	mciplayparms.dwFrom = 0 ; //���ֲ���ʱ��ͷ��ʼ
	mciSendCommand(DeviceId,MCI_PLAY,MCI_FROM|MCI_NOTIFY,(DWORD)(LPVOID)&mciplayparms) ;

}

void FileLoad(HWND hWnd,CString filepath)
{
	m_hWnd = hWnd;
	mciSendCommand(DeviceId,MCI_CLOSE,0,0);//�ڼ����ļ�ǰ�������һ�β��ŵ��豸
	mciopenparms.lpstrElementName = filepath ;//���������ļ�·�������豸

	DWORD myreturn ;
	if(myreturn =mciSendCommand(NULL,MCI_OPEN,MCI_OPEN_ELEMENT|MCI_WAIT,(DWORD)(LPVOID)&mciopenparms))
	{
		//������ʧ�ܣ���������Ϣ�����buffer�У������辯����ʾ
		char buffer[256] ;
		mciGetErrorString(myreturn,buffer,256);//��ȡ�������Ӧ�Ĵ�����Ϣ
		MessageBox(hWnd,buffer,"���󾯸棡",MB_ICONHAND|MB_ICONERROR|MB_ICONSTOP) ;//����������ʾ�Ի���
	}
	else
	{
		DeviceId = mciopenparms.wDeviceID;

	}

}

void CMymusic::OnBnClickedButton1() //���ֲ���
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString ws ;
	CComboBox* pfix = (CComboBox*)GetDlgItem(IDC_COMBO1) ;

	std::vector<CString>::iterator pmusic = myvec.end() ;
	std::vector<CString>::iterator pmusic2 = myvec.begin() ;

	int num ;
	num = -1 ;
	num = pfix->GetCurSel();
	if(num<0)//�ж������������б���ѡ��ĳһ��
	{
		if(alreadyadd == 1)//�жϸ����Ƿ��Ѿ����
		{

			pfix->GetWindowText(ws);
			CString temper ;
			int i ;
			for(i=0;i<20;i++)
			{
				pfix->GetLBText( i, temper);
				if(_tcscmp(ws,temper) == 0 )//�ҵ�����Ӹ����ľ���λ��
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
		if(strCBText == ws)//�жϴ�ʱ��Ӹ����Ƿ�͵�ǰѡ�и���һ��
		{
			CString road = *(pmusic2 + num) ;
			FileLoad(m_hWnd,road);
		}
		else
		{
			if(alreadyadd == 1)//�жϸ����Ƿ��Ѿ����
			{
				CString temper ;
				int i ;
				for(i=0;i<20;i++)
				{
					pfix->GetLBText( i, temper);
					if(_tcscmp(ws,temper) == 0 )//�ҵ�����Ӹ����ľ���λ��
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

void mypause()//�����豸ֹͣ����
{
	mciSendCommand(DeviceId,MCI_PAUSE,0,0) ;
}

void myresume()//�����豸�ָ�����
{
	mciSendCommand(DeviceId,MCI_RESUME,0,0) ;
}

void CMymusic::OnBnClickedButton2()   //��ͣ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString mystate ;
	CString cs ;
	GetDlgItemText(IDC_BUTTON2,mystate) ;//��ȡ��ťָʾ
	CString str1 = TEXT("��ͣ");
	CString str2 = TEXT("�ָ�");
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

void mystop()//�����豸ֹͣ���ţ�������Ϣ���
{
	mciSendCommand(DeviceId,MCI_STOP,0,0) ;
	mciSendCommand(DeviceId,MCI_CLOSE,0,0) ;
}

void CMymusic::OnBnClickedButton3()   //ֹͣ
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	mystop() ;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false) ;//���ź���ͣ����ѡ
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false) ;
}

DWORD setvolume(DWORD myvolume)//��������
{
	MCI_DGV_SETAUDIO_PARMS setvol ; //���������Ĳ����ṹ��
	setvol.dwCallback = NULL ;
	setvol.dwItem = MCI_DGV_SETAUDIO_VOLUME ;//��������
	setvol.dwValue = myvolume ;//����ֵ
	mciSendCommand(DeviceId,MCI_SETAUDIO,MCI_DGV_SETAUDIO_ITEM|MCI_DGV_SETAUDIO_VALUE,(DWORD)(LPVOID)&setvol) ;

	return 0 ;
}



void CMymusic::OnPaint()
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


void CMymusic::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMymusic::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CMymusic::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CString strLoad ;  
	CString filename ;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��
	{
		filename = m_music_listcontrol.GetItemText(pNMListView->iItem, 0);
		strLoad = m_music_listcontrol.GetItemText(pNMListView->iItem, 1); 
		SetDlgItemText(IDC_COMBO1,filename);
		FileLoad(m_hWnd,strLoad);
		musicplay();
	}		
	else
	{
		AfxMessageBox("˫�����ţ�����������ļ���");
		return;
	}
}


void CMymusic::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int index = pNMLV->iItem;
	if( index == -1 )
		return;
	HMENU hMenu = ::CreatePopupMenu();
	AppendMenu( hMenu, MF_STRING, 10001, _T("ɾ������") );
	CPoint pt;
	GetCursorPos(&pt); //��õ�ǰ���λ��
	UINT Cmd = (UINT)::TrackPopupMenu( hMenu, TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL );//�����˵�

	switch( Cmd )//��Ӧ����Ĳ˵�
	{
	case 10001:
		{
			POSITION p=m_music_listcontrol.GetFirstSelectedItemPosition();    //��ȡ��ѡ����λ��
			while (p) 
			{
				int  nSelected=m_music_listcontrol.GetNextSelectedItem(p); //��ȡѡ���е�����			
				CString strDeleteNum = m_music_listcontrol.GetItemText(nSelected,0) ;
				m_music_listcontrol.DeleteItem(nSelected); //��������ɾ��
				p = m_music_listcontrol.GetFirstSelectedItemPosition();  
			}
			break;
		}

	}
}
