// Receivefile.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyChattingroom_client.h"
#include "Receivefile.h"
#include "afxdialogex.h"


// CReceivefile �Ի���

IMPLEMENT_DYNAMIC(CReceivefile, CDialogEx)

CReceivefile::CReceivefile(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReceivefile::IDD, pParent)
{
	theApp.recve = this ;

	//�ļ���С
	m_dwFileSize = 0;
	//Ҫ������ļ�
	saveFilePath = TEXT("");
	saveFileFolder = TEXT("");
}

CReceivefile::~CReceivefile()
{
}

void CReceivefile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, progress1);
	DDX_Control(pDX, IDC_BUTTON2, m_openfile);
}



BEGIN_MESSAGE_MAP(CReceivefile, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CReceivefile::OnBnClickedButton2)
END_MESSAGE_MAP()


// CReceivefile ��Ϣ�������
//���������ļ�CSocket
void CReceivefile::CreateReceiveSoket()
{ 
	//TODO ���������ļ�CSocket
	theApp.serverSoc = new CReceiveServer();//�����ļ������
	if(!theApp.serverSoc)
	{
		MessageBox(_T("�ڴ治�㣡"));
		return;
	}

	if (!theApp.serverSoc->Create(1005,SOCK_STREAM,theApp.mylocalip))
	{
		AfxMessageBox(_T("�����ļ�ʱ����serverSoc����!"));

	}
	if (!theApp.serverSoc->Listen(1))
	{
		AfxMessageBox(_T(" serverSoc��������!"));

	}

}

HBRUSH CReceivefile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == this)
	{
		return m_Br2;
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_road||pWnd->GetDlgCtrlID() == IDC_STATIC_nam ||pWnd->GetDlgCtrlID() == IDC_STATIC_sizer||pWnd->GetDlgCtrlID() == IDC_RECV
		||pWnd->GetDlgCtrlID() == IDC_STATIC1)
	{
		pDC->SetBkMode(TRANSPARENT); //���ñ���Ϊ͸��
		hbr = HBRUSH(GetStockObject(HOLLOW_BRUSH));
		return hbr;;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CReceivefile::OnInitDialog()
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

	HBITMAP hbit_button2;
	hbit_button2 = (HBITMAP)::LoadImage(NULL,"../../picture/open_file.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION );

	((CButton*)GetDlgItem(IDC_BUTTON2))->SetBitmap(hbit_button2) ;

	font.CreateFont(15,0,0,0,FW_EXTRABOLD,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"����");
	GetDlgItem(IDC_STATIC_road)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_nam)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC_sizer)->SetFont(&font) ;
	GetDlgItem(IDC_STATIC1)->SetFont(&font) ;
	GetDlgItem(IDC_RECV)->SetFont(&font) ;

	m_openfile.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CReceivefile::OnBnClickedButton2()  //���ļ�Ŀ¼
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (this->saveFileFolder!="")
	{ 
		ShellExecute(NULL,_T("explore"),saveFileFolder, NULL, NULL, SW_SHOW);
	}
}


