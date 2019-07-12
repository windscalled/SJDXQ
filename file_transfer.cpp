// file_transfer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "file_transfer.h"
#include "afxdialogex.h"


// file_transfer �Ի���

IMPLEMENT_DYNAMIC(CFile_transfer, CDialogEx)

CFile_transfer::CFile_transfer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFile_transfer::IDD, pParent)
{
	theApp.sendFile = this ;
}

CFile_transfer::~CFile_transfer()
{
}

void CFile_transfer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, filepath);
	DDX_Control(pDX, IDC_BUTTON3, cancelsend);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}


BEGIN_MESSAGE_MAP(CFile_transfer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFile_transfer::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFile_transfer::OnBnClickedButton2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON3, &CFile_transfer::OnBnClickedButton3)
END_MESSAGE_MAP()


// file_transfer ��Ϣ�������




void CFile_transfer::OnBnClickedButton1()   //ѡ���ļ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	
	CFileDialog CFDlg(TRUE); 

	if (IDOK ==CFDlg.DoModal())     //���ļ��ɹ�
	{
		//ȡ���ļ�ȫ·������ʾ���༭��
		filepath.SetWindowText(CFDlg.GetPathName());

		//ȡ���ļ�����ʾ��IDC_STATIC
		SetDlgItemText(IDC_STATIC_name,_T("�ļ���: ")+CFDlg.GetFileName());

		CFile fileTemp;
		if(fileTemp.Open(CFDlg.GetPathName(),CFile::modeRead|CFile::shareDenyNone))
		{
			CString filesize;
			filesize.Format(_T("%d kb"),fileTemp.GetLength()/1024);
			SetDlgItemText(IDC_STATIC_size,filesize);
		}
		else
		{
			SetDlgItemText(IDC_STATIC_size,_T("δ֪"));
		}
		fileTemp.Close();
	}
	Invalidate() ;
}


void CFile_transfer::OnBnClickedButton2()   //�����ļ�
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bStopSend = FALSE; //�ָ�,��ʾ���жϷ����߳�

	CString m_strPath=_T("");
	filepath.GetWindowText(m_strPath);
	if (m_strPath=="")
	{
		AfxMessageBox(_T("��ѡ��Ҫ���͵��ļ�!"));
		return;
	}
	
	//ȡ�÷����ļ���С.���Ƶ�
	CFile file;
	if(!file.Open(m_strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("�ļ���ʧ��")); 
		return;
	}
	DWORD m_dwFileSize = file.GetLength();    //�ļ�����
	//CMessage *pMsg  = new CMessage(REQUEST,file.GetFileName(),m_dwFileSize);
	

	CHead myHead ;
	myHead.type = 8;		//�����ļ���־
	myHead.m_dwFileSize = m_dwFileSize ;
	strcpy_s(myHead.message,(CStringA)file.GetFileName()) ;
	strcpy_s(myHead.IP,(CStringA)theApp.mylocalip);
	strcpy_s(myHead.nicname,(CStringA)theApp.m_username);
	strcpy(myHead.fromID,(CStringA)theApp.m_userid);
	strcpy(myHead.toID,(CStringA)theApp.friendid);
	file.Close();
	//theApp.mytcp->SendMsg(pMsg) ;
	theApp.mytcp->Send(&myHead,sizeof(myHead));//�������ļ�
		
}


//���������߳�
UINT _SendThread(LPVOID lparam)
{  	
	CFile_transfer* pDlg=(CFile_transfer*)lparam;//ȡ��������ָ��
	//�����µ�Socket���ӽ��տͻ��˽����̴߳�����Socket
	//��ʼ��Csocket,���߳���ʹ��CSocket����Ҫ�ô˾�.�����ھ�̬MFC��������ʱ���ܳɹ�����socket 
	AfxSocketInit();
	CSocket sendSocket;
	
	if (!sendSocket.Create())
	{
		AfxMessageBox(_T("sendSocket.Create error"));
		return -1;
	}
	
	if (!sendSocket.Connect(theApp.otherlocalip,1005))
	{
		AfxMessageBox(_T("sendSocket.Connect error"));
		return -1;
	}
	//���Է����ļ���

	//ȡ���ļ� 
	CString m_strPath=_T("");
	pDlg->filepath.GetWindowText(m_strPath);

	//��Ҫ���͵��ļ�
	CFile file;
	if(!file.Open(m_strPath, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("�ļ���ʧ��")); 
		sendSocket.Close();
		return 1;
	}
	DWORD m_dwFileSize = file.GetLength();
	pDlg->m_ctrlProgress.SetRange32(0,m_dwFileSize);

	int nSize = 0, nLen = 0;
	DWORD dwCount = 0;
	char buf[512] = {0};
	file.Seek(0, CFile::begin);  
	//��ȡ�����Ͱ�ťΪ����
	pDlg->cancelsend.EnableWindow(TRUE);
	//��ʼ�����ļ�
	for(;;)
	{    

		//һ�ζ�ȡBLOCKSIZE��С���ļ�����
		nLen = file.Read(buf, 512);
		if(nLen == 0)
			break;

		//�����ļ�����
		nSize =sendSocket.Send(buf, nLen);

		dwCount += nSize;
		pDlg->m_ctrlProgress.SetPos(dwCount); 
		pDlg->SetDlgItemInt(IDC_STATIC_size,dwCount/1024);

		if (pDlg->bStopSend)//ֹͣ����
		{
			pDlg->Invalidate(TRUE) ;
			strcpy(buf,"cancle_send!");
			sendSocket.Send(buf, sizeof(buf));
			break;
		}
	}
	//�ر��ļ�
	file.Close();
	sendSocket.Close();

	if(m_dwFileSize == dwCount)
	{
		pDlg->Invalidate(TRUE) ;
		AfxMessageBox(_T("�ļ����ͳɹ�"));
	}
	else
	{
		AfxMessageBox(_T("�ļ�����ʧ��"));
	}
	//���ͽ��ȹ���
	pDlg->SetDlgItemInt(IDC_STATIC_size,0);
	pDlg->m_ctrlProgress.SetPos(0); 
	//��ȡ�����Ͱ�ťΪ������
	pDlg->cancelsend.EnableWindow(FALSE);


	return 0;
}



void CFile_transfer::sendFile()
{
	//���������ļ��߳�
	AfxBeginThread(_SendThread,this);
}


BOOL CFile_transfer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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

	HBITMAP hbit_button1, hbit_button2,hbit_button3;
	hbit_button1 = (HBITMAP)::LoadImage(NULL,"../../picture/surf.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/send.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );
	hbit_button3 = (HBITMAP)::LoadImage(NULL,"../../picture/cancle_send.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_BUTTON1))->SetBitmap(hbit_button1) ;
	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hbit_button3) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"����");
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_name)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_size)->SetFont(&font) ;

	cancelsend.EnableWindow(TRUE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


HBRUSH CFile_transfer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC1||pWnd->GetDlgCtrlID() == IDC_STATIC_name||pWnd->GetDlgCtrlID() == IDC_STATIC_size)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CFile_transfer::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bStopSend = TRUE ;
}
