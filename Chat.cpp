// Chat.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Chat.h"
#include "afxdialogex.h"



apple myapple ;
// CChat �Ի���

IMPLEMENT_DYNAMIC(CChat, CDialogEx)

CChat::CChat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChat::IDD, pParent)
	, m_recmessage(_T(""))
	, m_sendmsg(_T(""))
{

}

CChat::~CChat()
{
	
}

void CChat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_recmessage, m_recmsgctr);
	DDX_Text(pDX, IDC_recmessage, m_recmessage);
	DDX_Text(pDX, IDC_sendmsg, m_sendmsg);
}


BEGIN_MESSAGE_MAP(CChat, CDialogEx)
	ON_BN_CLICKED(IDC_send, &CChat::OnBnClickedsend)
	ON_BN_CLICKED(IDC_BUTTON2,OnCancel)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_file, &CChat::OnBnClickedfile)
	ON_BN_CLICKED(IDC_chatting, &CChat::OnBnClickedchatting)
END_MESSAGE_MAP()


// CChat ��Ϣ�������


void CChat::OnBnClickedsend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CTime time;
	time = CTime::GetCurrentTime();  //��ȡ����ʱ��

	CString strTime = time.Format("%Y-%m-%d %H:%M:%S  ");
	theApp.head.type = 3;
	UpdateData(true);
	chatting_record = strTime + (CString)"(" + theApp.m_userid + (CString)") " + theApp.m_username + (CString)"\r\n    " + m_sendmsg + (CString)"\r\n";
	m_recmessage += chatting_record ;
	CHead Msg;
	Msg.type = 3;
	
	strcpy_s(Msg.fromID,(CStringA)theApp.m_userid);
	strcpy_s(Msg.nicname,(CStringA)theApp.m_username);
	strcpy(Msg.id,(CStringA)theApp.m_userid);
	strcpy(Msg.toID,(CStringA)mytoid);
	strcpy_s(Msg.IP,theApp.head.IP);
	strcpy_s(Msg.message,(CStringA)m_sendmsg);

	//���ļ�
	CString mm = (CString)"10000" ;
	ofstream outfile;
	char messsage[1024] ;
	char repair[200];
	if(mytoid == mm) //Ⱥ�ļ�¼
	{
		//���ļ�				
		strcpy_s(myapple.mymesssage,(CStringA)chatting_record) ;  //��Ϣ
		strcpy(repair,"../MyChattingroom_client/����Ⱥ�������¼.txt");		
	}
	else //˽�ļ�¼
	{
		char tempname[100];
		char tempother[100] ;
		strcpy_s(tempname,(CStringA)theApp.m_username) ; //�ҵ��ǳ�
		strcpy_s(myapple.mymesssage,(CStringA)chatting_record) ;  //��Ϣ
		strcpy_s(tempother,(CStringA)theApp.m_client[(CString)Msg.toID]) ;  
		sprintf(repair,"../MyChattingroom_client/%s��%s�������¼.txt",tempname,tempother);		
	}
	outfile.open(repair,ios::binary | ios::app);
	outfile.write((char*)&myapple,sizeof(myapple));
	outfile.close();

	m_sendmsg = "";
	chatting_record = "";
	UpdateData(false);
	m_recmsgctr.LineScroll(m_recmsgctr.GetLineCount()-1);
	m_recmessage = "";
	theApp.mytcp->Send(&Msg,sizeof(Msg));
	
}


BOOL CChat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	mycaption =(CString)"����(" +  mytoid + (CString)") " + othername + (CString)" ������";
	SetWindowText(mycaption);//���öԻ������
	theApp.friendid = mytoid ;
	theApp.friendname = theApp.m_client[mytoid] ;
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

	HBITMAP hbit_button1, hbit_button2,hbit_button3,hbit_button4;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/file.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/record.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/send.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button4 = (HBITMAP)::LoadImage(NULL,"../../picture/close.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_file))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_chatting))->SetBitmap(hbit_button2) ;
	((CButton*)GetDlgItem(IDC_send))->SetBitmap(hbit_button3) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button4) ;


	return TRUE;  // return TRUE unless you set the focus to a control

	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CChat::GetErrMsg(CString& strOutErr)
{
// 	DWORD WINAPI FormatMessage(
// 		_In_     DWORD   dwFlags,
// 		_In_opt_ LPCVOID lpSource,
// 		_In_     DWORD   dwMessageId,
// 		_In_     DWORD   dwLanguageId,
// 		_Out_    LPTSTR  lpBuffer,
// 		_In_     DWORD   nSize,
// 		_In_opt_ va_list *Arguments
// 		);
	//char * pszErr = NULL;
	//if(!FormatMessage(FORMAT_MESSAGE_FROM_STRING |
	//	FORMAT_MESSAGE_ALLOCATE_BUFFER,
	//	NULL,
	//	GetLastError(),
	//	0,
	//	(LPWSTR)&pszErr, 
	//	0, 
	//	NULL))
	//{
	//	AfxMessageBox(TEXT("CChat::GetErrMsg����ȡ������Ϣʧ�ܣ�"));
	//	return FALSE;
	//}

	//strOutErr = pszErr;
	return TRUE;

}


HBRUSH CChat::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CChat::OnBnClickedfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(dlg.m_hWnd == 0)//�ж���û�д�����
	{

		dlg.Create(IDD_DIALOG4 ,this);

	}
	//ShowWindow(SW_HIDE);
	dlg.ShowWindow(SW_SHOW);
}


void CChat::OnBnClickedchatting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//theApp.cchat
	theApp.cchat = new CChatrecord();
	theApp.cchat->Create(IDD_DIALOG7,this) ;

	theApp.cchat->ShowWindow(SW_SHOW) ;
}


BOOL CChat::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnBnClickedsend();
			return TRUE ;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}




void CChat::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	DestroyWindow();   
}


void CChat::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::PostNcDestroy();
	theApp.m_usermessage.erase(mytoid);
	delete this ;
}
